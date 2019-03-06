#ifndef COLORVECTOR_H
#define COLORVECTOR_H

using namespace std;

class colorVector
{
    public:
        colorVector();
        float R,G,B,A;
        colorVector(float r,float g, float b, float a);
        colorVector& operator = (const colorVector &other);
        colorVector operator - (const colorVector &other) const;
        colorVector operator + (const colorVector &other) const;
        colorVector operator * (const float &scalar) const;
        colorVector operator / (const float &scalar) const;
    protected:
    private:
};

#endif // COLORVECTOR_H
