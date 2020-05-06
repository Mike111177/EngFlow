#include <iostream>

#include <Flow/blocks/codeblocks/LuaBlock.h>
#include <Flow/blocks/codeblocks/PythonBlock.h>

int main(){
	//Init block types
	Flow::PythonBlock::reg();
	Flow::LuaBlock::reg();

	//Create Blocks
	auto pyblock1 = Flow::Block::create("PythonBlock").value();
	auto pylogic1 = dynamic_cast<Flow::PythonBlock*>(pyblock1->logic());

	auto pyblock2 = Flow::Block::create("PythonBlock").value();
	auto pylogic2 = dynamic_cast<Flow::PythonBlock*>(pyblock2->logic());

	auto luablock1 = Flow::Block::create("LuaBlock").value();
	auto lualogic1 = dynamic_cast<Flow::LuaBlock*>(luablock1->logic());

	auto luablock2 = Flow::Block::create("LuaBlock").value();
	auto lualogic2 = dynamic_cast<Flow::LuaBlock*>(luablock2->logic());

	//Fill Blocks With Code
	pylogic1->setSource(
R"(
def createNumber():
    num = 3
    print("Python: Creating number {}".format(num))
    return num
)");

	lualogic1->setSource(
R"(
function squareNumber(num)
    nnum = num*num
    print(string.format("Lua: Squaring Number %s to %s", num,nnum))
    return nnum
end
return squareNumber
)");

	pylogic2->setSource(
R"(
def halfNumber(num):
    nnum = num/2
    print("Python: Halving number {} to {}".format(num, nnum))
    return nnum
)");

	lualogic2->setSource(
R"(
function getCos(num)
    nnum = math.cos(num)
    print(string.format("Lua: Getting cosine of number %s, it is %s", num, nnum))
    return nnum
end
return getCos
)");

	//Run
	std::cout << "Result (C++): " <<
		lualogic2->execute(
			pylogic2->execute(
				lualogic1->execute(
					pylogic1->execute({}))))
		<< std::endl;

	// Expected Output
	/* 
	Python: Creating number 3
	Lua: Squaring Number 3 to 9
	Python: Halving number 9.0 to 4.5
	Lua: Getting cosine of number 4.5, it is -0.21079579943078
	Result (C++): -0.210796
	*/
	return 0;
}