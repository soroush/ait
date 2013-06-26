/*
 * aabt-order.h
 *
 *  Created on: Jun 24, 2013
 *      Author: roya
 */

#ifndef AABT_ORDER_H_
#define AABT_ORDER_H_

#include "aabt-explanation.h"

namespace AIT {

struct CVOrderData {
	CVOrderData();
	CVOrderData(const CVOrderData& other);
	~CVOrderData();
	CVOrderData& operator=(const CVOrderData& c1);
	operator protocols::csp::aabt::P_CVOrderData() const;
	void readFromProtocol(const protocols::csp::aabt::P_CVOrderData&);

	AABT_Assignment a;
	std::vector<int> o;
	std::vector<int> tv;
	std::vector<AABT_Explanation> E;
};
} /* namespace AIT */
#endif /* AABT_ORDER_H_ */
