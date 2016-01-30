#pragma once

#include <nytl/vec.hpp>
#include <nytl/triangle.hpp>

namespace nytl
{

//sequence
template<std::size_t dim, typename prec, typename T = prec>
class sequence
{
public:
	struct point
	{
		Vec<dim, prec> position;
		T value;

		template<std::size_t odim>
		operator typename sequence<odim, prec, T>::point() const
		{
			return {Vec<odim, prec>(position), value};
		}

		template<std::size_t odim>
		point& operator=(const typename sequence<odim, prec, T>::point& p)
		{
			position = p.position;
			value = p.value;
		}
	};

protected:
	std::vector<point> points_;

public:
	void addPoint(const point& p);
	T valueAt(const Vec<dim, prec>& pos);
};

//util
template<std::size_t dim1, std::size_t dim2, typename prec, typename T>
float volumeOrSth(const Vec<dim1, typename sequence<dim2, prec, T>::point>& points)
{
	triangle<dim2, prec> tri(points[0].position, points[1].position, points[2].position);
	float A = tri.size();

	for(auto& p : points)
		std::cout << p.position << " -- ";

	std::cout << "\nvolume: " << A << "\n";
	return A;
}

template<std::size_t dim1, std::size_t dim2, typename prec, typename T> T 
interpolate(const Vec<dim1, typename sequence<dim2, prec, T>::point>& points,
	   	const Vec<dim2, prec>& pos)
{
	if(dim1 == 2)
	{
		std::cout << "point0 " << points[0].position << "\n";
		std::cout << "point1 " << points[1].position << "\n";
		std::cout << "point2 " << pos << "\n";
		float dist = distance(points[0].position, points[1].position);
		T ret;
		float fac = (1 - (distance(points[0].position, pos) / dist));
		std::cout << "fac0: " << fac << "\n";
		ret += fac * points[0].value;
		fac = (1 - (distance(points[1].position, pos) / dist));
		std::cout << "fac1: " << fac << "\n";
		ret += fac * points[1].value;
		std::cout << "rret: " << ret << "\n";
		return ret;
	}
	else
	{
		using point_type = typename sequence<dim2, prec, T>::point;
		auto myPoints = points;
		float volume = volumeOrSth<dim1, dim2, prec, T>(points);
		T ret;

		for(std::size_t i(0); i < dim1; ++i)
		{
			myPoints = points;
			myPoints[i] = myPoints.back();
			myPoints.back() = point_type{pos, T{}};

			float partVolume = volumeOrSth<dim1, dim2, prec, T>(myPoints);
			float fac = partVolume / volume;

			constexpr int newDim = (dim1 == 2) ? 2 : dim1 - 1;

			Vec<newDim, typename sequence<dim2, prec, T>::point> externalPoints;
			for(std::size_t o(0); o < dim1 - 1; ++o)
			{
				externalPoints[o].position = myPoints[o].position;
				externalPoints[o].value = myPoints[o].value;
			}

			ret += fac * interpolate<newDim, dim2, prec, T>(externalPoints, pos);
			std::cout << "ret: " << ret << "\n";
		}

		return ret;
	}
}

//implementation
template<std::size_t dim, typename prec, typename T>
void sequence<dim, prec, T>::addPoint(const sequence<dim, prec, T>::point& p)
{
	points_.push_back(p);
}

template<std::size_t dim, typename prec, typename T>
T sequence<dim, prec, T>::valueAt(const Vec<dim, prec>& pos)
{
	if(points_.empty()) return T{};

	Vec<dim + 1, point*> pointsArea;
	pointsArea.fill(nullptr);

	for(auto& p : points_)
	{
		if(!pointsArea[0] && p.position[0] >= pos[0])
		{
			pointsArea[0] = &p;
			continue;
		}
		if(!pointsArea[1] && p.position[0] <= pos[0])
		{
			pointsArea[1] = &p;
		}
	}

	if(!pointsArea[0] || !pointsArea[1])
	{
		return T{};
		//clamp!
	}

	for(std::size_t i(1); i < dim; ++i)
	{
		int mustBe = 0;
		if(pointsArea[0]->position[i] < pos[i] && pointsArea[1]->position[i] < pos[i]) 
			mustBe = 1;
		else if(pointsArea[0]->position[i] > pos[i] && pointsArea[1]->position[i] > pos[i]) 
			mustBe = 2;

		for(auto& p : points_)
		{
			for(auto* pa : pointsArea)
			{
				if(pa == &p) goto contin;
			}

			if(!mustBe || ((p.position[i] > pos[i]) && (mustBe == 1)) || 
					(p.position[i] < pos[i] && (mustBe == 2)))
			{
				pointsArea[i + 1] = &p;
				break;
			}

			return T{};
			//clamp!

			contin:
			continue;
		}	
	}

	//http://answers.unity3d.com/questions/383804/calculate-uv-coordinates-of-
	//3d-point-on-plane-of-m.html
	Vec<dim + 1, point> iPoints;
	for(std::size_t i(0); i < dim + 1; ++i)
		iPoints[i] = *pointsArea[i];
			
	return interpolate<dim + 1, dim, prec, T>(iPoints, pos);
}

}
