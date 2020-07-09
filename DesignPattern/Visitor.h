#pragma once

namespace JSLib {

	class AcyclicVisitor {
	public:
		virtual ~AcyclicVisitor() {}
	};

	template <class T>
	class Visitor {
	public:
		virtual ~Visitor() {}
		virtual void visit(T&) = 0;
	};

}