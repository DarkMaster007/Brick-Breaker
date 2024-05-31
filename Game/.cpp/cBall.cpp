#include "cBall.h"

int cBall::ballCount = 0;
Texture2D cBall::texBall;
extern int frame;

cBall::cBall(int posX, int posY, int ballSize, float ballSpeed):direction(STOP) {
    setSize(ballSize);
    originalSize = getSize();
    setX(posX);
    setY(posY);
    originalX = x;
    originalY = y;
    angle = 45.0f;
    speedX = std::abs(ballSpeed * std::cos(angle));
    speedY = std::abs(ballSpeed * std::sin(angle));
    originalSpeedX = speedX;
    originalSpeedY = speedY;
    acceleration = 0.017;
    randomizeMovement();
    ballCount++;
}

cBall::cBall(int posX, int posY, int ballSize, float ballSpeed, eDir newDirection):cBall(posX, posY, ballSize, ballSpeed) {
    direction = newDirection;
}

cBall::cBall(const cBall& other)
    : x(other.x), y(other.y), originalX(other.originalX), originalY(other.originalY),
      currentSize(other.currentSize), originalSize(other.originalSize), angle(other.angle),
      speedX(other.speedX), speedY(other.speedY), originalSpeedX(other.originalSpeedX),
      originalSpeedY(other.originalSpeedY), acceleration(other.acceleration), direction(other.direction) {
    randomMovementOffset[0] = other.randomMovementOffset[0];
    randomMovementOffset[1] = other.randomMovementOffset[1];
    ballCount++;
}

cBall::cBall(cBall&& other) noexcept
    : x(other.x), y(other.y), originalX(other.originalX), originalY(other.originalY),
      currentSize(other.currentSize), originalSize(other.originalSize), angle(other.angle),
      speedX(other.speedX), speedY(other.speedY), originalSpeedX(other.originalSpeedX),
      originalSpeedY(other.originalSpeedY), acceleration(other.acceleration), direction(other.direction) {
    randomMovementOffset[0] = other.randomMovementOffset[0];
    randomMovementOffset[1] = other.randomMovementOffset[1];
    other.x = 0;
    other.y = 0;
    ballCount++;
}

cBall& cBall::operator=(cBall&& other) noexcept {
    if (this != &other) {
        // Move data from 'other' to 'this'
        x = other.x;
        y = other.y;
        originalX = other.originalX;
        originalY = other.originalY;
        currentSize = other.currentSize;
        originalSize = other.originalSize;
        angle = other.angle;
        speedX = other.speedX;
        speedY = other.speedY;
        originalSpeedX = other.originalSpeedX;
        originalSpeedY = other.originalSpeedY;
        acceleration = other.acceleration;
        direction = other.direction;
        std::copy(std::begin(other.randomMovementOffset), std::end(other.randomMovementOffset), std::begin(randomMovementOffset));
        marsenneTwister = std::move(other.marsenneTwister);
        randomNrDistribution = std::move(other.randomNrDistribution);
        randomNrDirection = std::move(other.randomNrDirection);

        // Invalidate the moved-from state
        other.x = 0;
        other.y = 0;
    }
    return *this;
}

cBall::~cBall() {
    ballCount--;
}

void cBall::Reset() {
    x = originalX;
    y = originalY;
    direction = STOP;
    currentSize = originalSize;
    speedX = originalSpeedX;
    speedY = originalSpeedY;
}

void cBall::randomizeMovement() {
    randomMovementOffset[0] = randomNrDistribution(marsenneTwister) / 10.0f;
    randomMovementOffset[1] = randomNrDistribution(marsenneTwister) / 10.0f;
}

void cBall::Move() {
    float frameTime = fmod(GetFrameTime(), 1.0f);
    if (direction != STOP) {
        float speedLimit = 1000;
        if (speedY < std::abs(speedLimit * std::sin(angle))) {
            speedY += speedY * acceleration * frameTime;
        } else {
            speedY += std::abs(speedLimit * std::sin(angle)) * frameTime;
        }
        if (speedX < std::abs(speedLimit * std::cos(angle))) {
            speedX += speedX * acceleration * frameTime;
        } else {
            speedX += std::abs(speedLimit * std::cos(angle)) * frameTime;
        }
        switch (direction) {
        case UPLEFT:
            x -= (speedX + randomMovementOffset[0]) * frameTime;
            y -= (speedY + randomMovementOffset[1]) * frameTime;
            break;
        case DOWNLEFT:
            x -= (speedX + randomMovementOffset[0]) * frameTime;
            y += (speedY + randomMovementOffset[1]) * frameTime;
            break;
        case UPRIGHT:
            x += (speedX + randomMovementOffset[0]) * frameTime;
            y -= (speedY + randomMovementOffset[1]) * frameTime;
            break;
        case DOWNRIGHT:
            x += (speedX + randomMovementOffset[0]) * frameTime;
            y += (speedY + randomMovementOffset[1]) * frameTime;
            break;
        default:
            direction = STOP;
            break;
        }
    }
}
