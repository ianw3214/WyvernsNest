#include <array>

class Camera {
public:
    Camera();
    void update();
    void setPosition(float x, float y, float z);

    std::array<float, 3> getPosition() const { return position; }
    std::array<float, 16> getViewMatrix() const { return viewMatrix; }
private:
    std::array<float, 16> viewMatrix;
    std::array<float, 3> position;
};