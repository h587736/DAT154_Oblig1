#include "stdafx.h"
#include "Car.h"




Car::Car(int carNr)
{
	Car::id = carNr;
}

Car::Car(const Car& car)
{
	this->id = car.nr;
	this->bottom = car.bottom;
	this->left = car.left;
	this->right = car.right;
	this->top = car.top;
	this->direction = car.direction;
}

Car::~Car()
{
}

int Car::getId()
{
	return Car::nr;
}

int Car::getDirection()
{
	return Car::direction;
}

int Car::getTop()
{
	return Car::top;
}

int Car::getLeft()
{
	return Car::left;
}

int Car::getRight()
{
	return Car::right;
}

int Car::getBottom()
{
	return Car::bottom;
}

void Car::setTop(int t)
{
	Car::top = t;
}

void Car::setLeft(int l)
{
	Car::left = l;
}

void Car::setRight(int r)
{
	Car::right = r;
}

void Car::setBottom(int b)
{
	Car::bottom = b;
}

void Car::setDirection(int r) {
	Car::direction = r;
}

bool Car::operator==(Car& car1) const
{
	bool result = this->nr == car1.getNr();
	return result;
}

bool operator==(Car& car1, Car& car2)
{
	return car1.getNr() == car2.getNr();
}