#ifndef ASGETREK_PANEL_HPP
#define ASGETREK_PANEL_HPP
#include "gamelib/GameObject.hpp"

class Panel : public GameObject
{
 public:
  Panel()  = default;
  ~Panel() = default;

  void createPanelWithText(GameObject& gameObject, ASGE::Text& text);
  void createPanelWithoutText(
    GameObject& obj, ASGE::Colour primary, ASGE::Colour secondary);
  void setPanelSize(GameObject& gameObject, float size);
  void setPos(float x, float y);
  void renderPanel(ASGE::Renderer* renderer);
  float getXPos();
  float getYPos();
  void centralisePanelText(ASGE::Text& text);
  void centralisePanelText(ASGE::Text& text, float offset);

 private:
  float PADDING             = 10.0F;
  ASGE::Colour PANEL_COLOUR = ASGE::COLOURS::BLACK;
};

struct PanelSetup
{
  GameObject container;
  Panel panel;
  ASGE::Text title{}, text{};
  std::vector<std::unique_ptr<ASGE::Text>> text_vec{};
};

#endif // ASGETREK_PANEL_HPP
