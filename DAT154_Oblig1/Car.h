#pragma once
class Car
{
public:
	Car(int carNr);
	Car(const Car& car);
	~Car(); 


	int getNr();
	int getDirection();
	int getTop();
	int getLeft();
	int getRight();
	int getBottom();

	void setTop(int t);
	void setLeft(int l);
	void setRight(int r);
	void setBottom(int b);
	void setDirection(int r);

	bool operator==(Car& car1) const;
	friend bool operator == (Car const& car1, Car const& car2);

private:
	int nr;
	int top;
	int left;
	int right;
	int bottom;
	int direction;
};
