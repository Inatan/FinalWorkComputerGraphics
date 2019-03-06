#include "colorVector.h"

colorVector::colorVector()
{
    R=G=B=0.0f;
    A=1.0f;
}

colorVector::colorVector(float r,float g, float b, float a)
{
    R=r;
    G=g;
    B=b;
    A=a;
}

colorVector& colorVector::operator = (const colorVector &other){
    R=other.R;
    G=other.G;
    B=other.B;
    A=other.A;
    return *this;
}

colorVector colorVector::operator - (const colorVector &other) const{
    return colorVector(R-other.R,G-other.G,B-other.B,A-other.A);
}

colorVector colorVector::operator + (const colorVector &other) const{
    return colorVector(R+other.R,G+other.G,B+other.B,A+other.A);
}

colorVector colorVector::operator * (const float &scalar) const{
    return colorVector(R*scalar,G*scalar,B*scalar,A*scalar);
}

colorVector colorVector::operator / (const float &scalar) const{
    return colorVector(R/scalar,G/scalar,B/scalar,A/scalar);
}
