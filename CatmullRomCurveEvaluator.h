#ifndef INCLUDED_CATMULLROM_CURVE_EVALUATOR_H
#define INCLUDED_CATMULLROM_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"
#include "vec.h"
#include "mat.h"

//using namespace std;

class CatmullRomCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
private:
	const Mat4f CAMULLROM_MATRIX = Mat4f(-1.0, 3.0, -3.0, 1.0, 2.0, -5.0, 4.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 2.0, 0.0, 0.0) / 2.0;
};

#endif