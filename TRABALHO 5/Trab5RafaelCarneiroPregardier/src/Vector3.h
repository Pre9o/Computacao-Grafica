#pragma once
// Conteúdo do arquivo de cabeçalho

#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include <math.h>
#include <stdio.h>

class Vector3
{
public:
   float x, y, z;

   Vector3()
   {
      x = y = z = 0;
   }

   Vector3(float _x, float _y, float _z)
   {
       x = _x;
       y = _y;
       z = _z;
   }

   void set(float _x, float _y, float _z)
   {
       x = _x;
       y = _y;
       z = _z;
   }

   void normalize()
   {
       float norm = (float)sqrt(x*x + y*y + z*z);

       if(norm==0.0)
       {
          printf("\n\nNormalize::Divisao por zero");
          x = 1;
          y = 1;
          z = 1;
          return;
       }
       x /= norm;
       y /= norm;
       z /= norm;
   }

   Vector3 operator - (const Vector3& v)
   {
        Vector3 aux( x - v.x, y - v.y, z - v.z);
        return( aux );
   }

   Vector3 operator + (const Vector3& v)
   {
       Vector3 aux( x + v.x, y + v.y, z + v.z);
       return( aux );
   }

   //Adicionem os demais overloads de operadores aqui.

   Vector3 operator * (const float& v)
   {
       Vector3 aux( x * v, y * v, z * v);
       return( aux );
   }

    Vector3 operator / (const float& v)
    {
         Vector3 aux( x / v, y / v, z / v);
         return( aux );
    }

    Vector3 operator= (const Vector3& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    Vector3 cross (const Vector3& v)
    {
        Vector3 aux( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
        return( aux );
    }


};

#endif
