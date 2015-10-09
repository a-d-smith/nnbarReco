#include<cmath>
class vector3{
	// Define friends of class
	friend vector3 operator+(double scalar, const vector3 &v);
	// Allow our overloaded operator<< function to be a friend of vector3
	friend ostream & operator<<(ostream &os, const vector3 &v);
	private:
		double x,y,z; 
	public:
		// Constructors and destructor
 		vector3(){x=y=z=0;} 
		vector3(double xin, double yin, double zin){x=xin; y=yin; z=zin;} 
		~vector3(){} 
		// Access functions to set and get vector components
 		void setx(double xin) {x=xin;}
                void sety(double yin) {y=yin;}
                void setz(double zin) {z=zin;}
                double getx() const {return x;}
                double gety() const {return y;}
  		double getz() const {return z;}
                // Function to overload + operator (addition of two vectors)
                vector3 operator+(const vector3 &v) const{
  	              vector3 temp(x+v.getx(),y+v.gety(),z+v.getz());
                      return temp;
                }
		// Function to get the distance between two points
		double distTo(const vector3 &v) const{
			return pow( pow(x-v.getx(),2) + pow(y-v.gety(),2) + pow(z-v.getz(),2), 0.5);
		}
};
