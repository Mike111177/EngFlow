#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <string>
#include <unordered_map>

#include <Flow/FlowTypes.h>

namespace Flow {
    template <typename T>
    concept LogicType_c = requires (T) { { T::LogicType }->std::string; };

    //User facing code unit, must have an associated logic type
    class Block final {
    public:
        //Define block behavior in derived class of Logic
        class Logic;

        std::string name; //Name of block
        Logic* logic();
        Logic* const logic() const;
        std::string const& type() const;

        //Use this to instantiate a block
        static std::optional<std::shared_ptr<Block>> create(std::string type);

        //Usage: registerLogic<Flow::PythonBlock>();
        template <LogicType_c L>
        static void registerLogic() { Registry.insert({ L::LogicType, constructBlock<L> }); }
    private:
        std::string _type;
        std::unique_ptr<Logic> b_logic;
        Block(std::string const& type, std::unique_ptr<Logic>& logic);
        Block(); //Constructor should only be run by constructBlock via create(std::string type)

        template<typename L>
        static std::shared_ptr<Block> constructBlock() {
            std::shared_ptr<Block> ptr(new Block);
            *ptr = Block(L::LogicType, std::unique_ptr<Logic>(new L(ptr)));
            return ptr;
        }

        //Registry used to lookup block types
        static std::unordered_map<std::string, decltype(&constructBlock<Logic>)> Registry;
    };

    class Block::Logic {
        const std::weak_ptr<Block> _block;
    public:
        Logic(std::weak_ptr<Block>);
        Block& block();
        virtual FlowResourceList& saveResources(FlowResourceList&);
        virtual void loadResources(FlowResourceList&);
        virtual ~Logic();
    };
}