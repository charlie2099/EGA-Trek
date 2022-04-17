#ifndef ASGETREK_MENUSCENE_HPP
#define ASGETREK_MENUSCENE_HPP
#include "SceneManager.hpp"

class MenuScene : public SceneManager
{
 public:
  bool init(ASGE::Renderer* renderer) override;
  void keyInputs(ASGE::SharedEventData data);
  void update();
  void render(ASGE::Renderer* renderer);

  void setDifficulty(size_t difficulty);
  [[nodiscard]] size_t getDifficulty() const;

  void setPassword(std::string password);
  [[nodiscard]] std::string getPassword() const;

  void setName(std::string name);
  [[nodiscard]] std::string getName() const;

 private:
  void briefInputOptions();
  void usernameInputOptions();
  void difficultyInputOptions();
  void passwordInputOptions();
  void assigningInputBoxToState();
  void textAlign();               // TODO: Potentially move to SceneManager later
  void activeQuestionHighlight(); // TODO: Potentially move to SceneManager later
  void initBriefPanel();
  void initQuestionText();
  void briefPages();
  void updateBriefPages();
  void makeBriefControlsActive(const ASGE::KeyEvent* key);
  void resetBriefPagesCounter();
  void nextQuestion();
  void generalKeyInputs(const ASGE::KeyEvent* key);
  void initBriefText(float x, float y);
  void initRightArrowButton(ASGE::Renderer* renderer, float x, float y);
  void initUserInputText();
  void initBriefTitleText();
  std::string NAME, PASSWORD;
  size_t DIFFICULTY    = 0;
  size_t brief_counter = 0;
};

#endif // ASGETREK_MENUSCENE_HPP
