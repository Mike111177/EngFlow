#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <future>
#include <variant>

#include <Flow\blocks\AbstractCodeBlock.h>

//TODO unexecptionafy this
using CompileResult = std::variant<bool, std::runtime_error>;

struct Flow::AbstractCodeBlock::PData {
	bool compiling = false;
	std::mutex sourceLock;
	std::condition_variable compcv;
	CompileResult compiled = false;
};

Flow::AbstractCodeBlock::AbstractCodeBlock(std::weak_ptr<Block> b) : Logic(b), pdata(std::make_unique<PData>()) {}

class Flow::AbstractCodeBlock::CompileThread {
	class TData {
		bool running = true;
		bool waiting = false;
		std::queue<Flow::AbstractCodeBlock*> queue;
		std::mutex qm;
		std::mutex idle;
		std::condition_variable idleWait;
	public:
		bool shouldRun() {
			return running;
		}
		bool shouldBeIdle() {
			std::lock_guard<std::mutex> lock(qm);
			return queue.empty() && shouldRun();
		}
		void waitForActivity() {
			waiting = true;
			std::unique_lock<std::mutex> lk(idle);
			idleWait.wait(lk, [this]{return !shouldBeIdle();});
			waiting = false;
		}
		void notify() { idleWait.notify_all(); }
		Flow::AbstractCodeBlock* next() {
			std::lock_guard<std::mutex> lock(qm);
			if (queue.empty()) {
				return nullptr;
			} else {
				auto n = queue.front();
				queue.pop();
				return n;
			}
		}
		void add(Flow::AbstractCodeBlock* block) {
			std::lock_guard<std::mutex> lock(qm);
			queue.emplace(block);
		}
		void stop() {
			running = false;
			notify();
		}
	};
	std::shared_ptr<TData> data;
	std::thread thread;
	static void loop(std::shared_ptr<TData> data) {
		while (data->shouldRun()) {
			if (data->shouldBeIdle()) data->waitForActivity();
			else if (auto wi = data->next()) {
				try {
					wi->pdata->compiled = wi->precompile();
				} catch (std::runtime_error e) {
					wi->pdata->compiled = e;
				}
				wi->pdata->compiling = false;
				wi->pdata->compcv.notify_all();
			}
		}
	}
public:
	CompileThread() : data(std::make_shared<TData>()), thread(loop, data) {}
	~CompileThread() {
		data->stop();
		thread.join();
	}
	void addToQueue(Flow::AbstractCodeBlock* block) {
		block->pdata->compiling = true;
		data->add(block);
	}
	void notify() { data->notify(); }
};

Flow::FlowResourceList& Flow::AbstractCodeBlock::saveResources(FlowResourceList &resList) {
	resList.emplace_back("source" + sourceExt, std::vector<char>{ source.begin(), source.end() });
	return resList;
}

void Flow::AbstractCodeBlock::loadResources(FlowResourceList& resources) {
	for (auto& res : resources) {
		if (res.name == "source" + sourceExt) {
			setSource({ res.data.data(), res.data.data() + res.data.size() });
			break;
		}
	}
}

std::vector<std::string> Flow::AbstractCodeBlock::params() {
	waitReady();
	return nparams();
}

void Flow::AbstractCodeBlock::waitReady() {
	if (pdata->compiling) {
		compiler->notify();
		std::unique_lock<std::mutex> lk(pdata->sourceLock);
		pdata->compcv.wait(lk, [this] {return !pdata->compiling; });
	}
}

//TODO make dedicated function for waiting for compile
Flow::FlowVar Flow::AbstractCodeBlock::execute(FlowVar args) {
	waitReady();
	if (std::holds_alternative<bool>(pdata->compiled) && !std::get<bool>(pdata->compiled)) {
		throw std::runtime_error("Codeblock is not compiled yet!");
	} else if (std::holds_alternative<std::runtime_error>(pdata->compiled)){
		throw std::get<std::runtime_error>(pdata->compiled);
	}
	return run(args);
}

void Flow::AbstractCodeBlock::setSource(std::string new_source) {
	waitReady();
	source = new_source;
	compiler->addToQueue(this);
}

Flow::AbstractCodeBlock::~AbstractCodeBlock() {}
