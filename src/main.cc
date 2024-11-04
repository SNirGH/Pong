#include <SFML/Graphics.hpp>
#include <algorithm>

constexpr float PADDLE_WIDTH = 16.0f;
constexpr float PADDLE_HEIGHT = 64.0f;
constexpr float BALL_SIZE = 16.0f;
constexpr float PADDLE_SPEED = 200.0f;
constexpr float BALL_SPEED = 200.0f;

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

  sf::RectangleShape player(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
  player.setPosition(50.0f, window.getSize().y / 2 - PADDLE_HEIGHT / 2);

  sf::RectangleShape ball(sf::Vector2f(BALL_SIZE, BALL_SIZE));
  sf::Vector2f OriginalBallPos = {window.getSize().x / 2 - BALL_SIZE / 2, window.getSize().y / 2 - BALL_SIZE / 2};
  ball.setPosition(OriginalBallPos);

  sf::RectangleShape computer(sf::RectangleShape(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT)));
  computer.setPosition(window.getSize().x - 50.0f, window.getSize().y / 2 - PADDLE_HEIGHT / 2);

  sf::Vector2f ballVelocity(BALL_SPEED, BALL_SPEED);
  sf::Clock clock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    float deltaTime = clock.restart().asSeconds();

    ball.move(ballVelocity * deltaTime);

    sf::Vector2f ballPosition = ball.getPosition();
    ballPosition.x = std::clamp(ballPosition.x, 0.0f, window.getSize().x - BALL_SIZE);
    ballPosition.y = std::clamp(ballPosition.y, 0.0f, window.getSize().y - BALL_SIZE);
    ball.setPosition(ballPosition);

    if (ball.getPosition().y <= 0 || ball.getPosition().y >= window.getSize().y - BALL_SIZE) {
      ballVelocity.y = -ballVelocity.y;
    }
    if (ball.getPosition().x <= 0 || ball.getPosition().x >= window.getSize().x - BALL_SIZE) {
      ball.setPosition(OriginalBallPos);
      ballVelocity.x = -ballVelocity.x;
    }

    if (ball.getGlobalBounds().intersects(player.getGlobalBounds()) ||
        ball.getGlobalBounds().intersects(computer.getGlobalBounds())) {
      ballVelocity.x = -ballVelocity.x;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      player.move(0, -PADDLE_SPEED * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      player.move(0, PADDLE_SPEED * deltaTime);
    }

    sf::Vector2f playerPosition = player.getPosition();
    playerPosition.y = std::clamp(playerPosition.y, 0.0f, window.getSize().y - PADDLE_HEIGHT);
    player.setPosition(playerPosition);

    sf::Vector2f computerPosition = computer.getPosition();
    if (ball.getPosition().y < computerPosition.y) {
      computer.move(0, -PADDLE_SPEED * deltaTime);
    } else if (ball.getPosition().y > computerPosition.y + PADDLE_HEIGHT) {
      computer.move(0, PADDLE_SPEED * deltaTime);
    }
    window.clear();
    window.draw(player);
    window.draw(ball);
    window.draw(computer);
    window.display();
  }

  return 0;
}
