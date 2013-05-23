/*
 * aabt-solver.hpp
 *
 *  Created on: May 22, 2013
 *      Author: roya
 */

#ifndef AABT_SOLVER_HPP_
#define AABT_SOLVER_HPP_

#include <vector>
using namespace std;


template<typename V, typename T>
namespace AIT {

class AABT_Solver
{

	enum class messageType {
		OK, NOGOOD, ORDER, STOP,
	};
	struct Explanation{
		CompoundAssignment<V,T> SE;
		int currentDomainlenght;
	};

	class Message {
			friend class boost::serialization::access;
		public:
			Message();
/*ok*/		Message(const messageType&, const AgentID&,
					const Assignment<AgentID, T>&,
					const Explanation,
					const int tv,
					const std::vector<AgentID>,
					const std::vector<int>);

/*order*/			Message(const messageType&, const AgentID&,
						const vector<AgentID>&,
						const vector<int>);

/*nogood*/			Message(const messageType&, const AgentID&,
					const CompoundAssignment<AgentID, T>&,
					const std::vector<AgentID>,
					const std::vector<int>);

/*stop*/           Message(const messageType&, const AgentID&);
			messageType type;
			AgentID sender;
			Assignment<AgentID, T> assignment;
			CompoundAssignment<AgentID, T> nogood;
			std::vector<AgentID> order;
			std::vector<int> terminationvalue;
			Explanation exp;

			template<class Archive>
			void serialize(Archive & ar, const unsigned int version) {
				ar & type;
				ar & sender;
				ar & assignment;
				ar & nogood;
			}
		};

	struct nogood
	{
		V* lhvarible;
		T* lhvalue;
		V* rhvarible;
		T* rhvalue;
    };
	private:
		T* myValue;
		void getAgentList();
		AgentID id;
		std::list<AgentID> preceding; // Γ+
		std::list<AgentID> succeeding; // Γ-
		CompoundAssignment<V, T> myAgentView;
		std::vector<AgentID> order;
		int timestamp;
		std::vector<int> terminationvalue;
		Explanation exp;
		std::list<V,Explanation> scagentsExp;
		std::list<nogood> nogoodstore;



};
}

#endif /* AABT_SOLVER_HPP_ */
