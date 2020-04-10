namespace Flow{
	class FlowType;
	class FlowVar{
	public:
		virtual FlowType* getType() = 0;
		virtual ~FlowVar() = 0;
	};
	class FlowType{
	public:
		virtual FlowVar * create() = 0;
		virtual ~FlowType() = 0;
	};
	class FlowTypeConstraint{
	public:
		virtual bool test(FlowType*) = 0;
		virtual bool test(FlowVar*);
	};
};