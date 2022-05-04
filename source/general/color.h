#pragma once
#include "includes.h"


class Color {
public:
    
    Color(unsigned char r,unsigned char g,unsigned char b);
    Color();

    /**
     * 
     * @return the color as floats from 0 to 1
     */
    glm::vec3& Normalized();
    /**
     * 
     * @return the color as integers from 0 to 255
     */
    glm::ivec3 AsIntegers() const;

    ImVec4 AsImVec4();
    
    Color& operator+= (const Color& other);
    Color& operator-= (const Color& other);

    Color operator+ (const Color& other);

    Color operator- (const Color& color);

    /**
    * 
     * Sets the color from rgb values normalized
     */
    void Set(float r,float g,float b);
    /**
     * Sets the color from rgb values from 0 to 255
     */
    void Set(unsigned char r,unsigned char g,unsigned char b);
    
    /**
     * A function to modify the color value as needed
     * @param  changeFunction -> a lambda or function to use for changing (take a glm::vec3 as parameter for the color(0-1))
     */
    void Modify(std::function<void(glm::vec3&)> changeFunction);
    
    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
    

private:
    glm::vec3 m_ColorValue = glm::vec3(0,0,0);


};