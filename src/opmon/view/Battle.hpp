/*!
 * \file Battle.hpp
 * \authors Cyrielle
 * \authors Navet56
 * \copyright GNU GPL v3.0
*/
#pragma once

#include "../../utils/CycleCounter.hpp"
#include "../model/objects/Turn.hpp"
#include "../model/storage/BattleData.hpp"
#include "../start/Core.hpp"
#include "Dialog.hpp"
#include "Elements.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <queue>

namespace OpMon {

    namespace Model {
        class OpTeam;
        class BattleData;
    } // namespace Model

    namespace View {

        /*!
         * \brief Class managing the battles on the screen.
         *
         * The Battle view alternates between two phases :
         * - The choices phase, with the different menus where the player has to choose between different actions and attacks. The view will only manage these two choices, the other selection menus (OpMon and items) will be manages by other views.
         * - The turns phase, in which the actions are being executed. Everything is calculated in BattleCtrl and stored in a queue of TurnAction. This class reads these objects corresponding to the different animations this view has to do according to the TurnActionType object in them.
         */
        class Battle {
          private:
            /*!
             * \brief The sprite of the front OpMon.
             */
            sf::Sprite atk;
            /*!
             * \brief The sprite of the opposite OpMon.
             */
            sf::Sprite def;

            /*!
             * \brief The sf::Transform used to animate the front OpMon.
             */
            sf::Transform atkTr;
            /*!
             * \brief The sf::Transform used to animate the opposite OpMon.
             */
            sf::Transform defTr;

            /*!
             * \brief A pointer to the player's team.
             */
            const Model::OpTeam *atkTeam;
            /*!
             * \brief A pointer to the opponent's team.
             */
            const Model::OpTeam *defTeam;

            /*!
             * \brief The current queue of OpMon animations.
             */
            std::queue<Transformation> *currentOpAnims = nullptr;

            /*!
             * \brief The HP of the front OpMon.
             * \details -1 means unitialised.
             */
            int atkHp = -1;
            /*!
             * \brief The HP of the opposite OpMon.
             * \details -1 Ã¹eans unitialised.
             */
            int defHp = -1;

            /*!
             * \brief The number of the current turn.
             * \details During one battle turn, it can be the turn of the turn of one OpMon or another. 0 is the turn of the fastest OpMon, 1 the other's turn.
             */
            int turnNber = 0;

            /*!
             * \brief If the turn has been launched and calculated in BattleCtrl.
             */
            bool turnLaunched = false;

            sf::Sprite background;
            sf::Sprite playerSpr;
            sf::Sprite trainerSpr;
            sf::Sprite dialogSpr;
            /*!
             * \brief The four choices in the battles (used for the action and attack selection.)
             */
            sf::Text choicesTxt[4];
            /*!
             * \brief The position of the four choices (Battle::choicesTxt)
             */
            sf::Vector2f posChoices[4];
            /*!
             * \brief The cursor used to select the choices.
             */
            sf::Sprite cursor;
            /*!
             * \brief The little dialog next to the action selection.
             */
            sf::Text waitText;

            /*!
             * \brief The names of the OpMons.
             */
            sf::Text opName[2];
            /*!
             * \brief The levels of the OpMons.
             */
            sf::Text opLevel[2];
            /*!
             * \brief The HP of the front OpMon.
             */
            sf::Text opHp;

            /*!
             * \brief The depleted health bar (in black & white).
             */
            sf::Sprite healthbar1[2];
            /*!
             * \brief The coloured health bar.
             */
            sf::Sprite healthbar2[2];
            /*!
             * \brief The background of the infobox of the player's OpMon.
             */
            sf::Sprite infoboxPlayer;
            /*!
             * \brief The background of the infobox of the opponent's OpMon.
             */
            sf::Sprite infoboxTrainer;
            sf::Sprite shadowTrainer;
            sf::Sprite shadowPlayer;

            /*!
             * \brief The choice the cursor is currently on.
             */
            Utils::CycleCounter curPos = Utils::CycleCounter(4);

            /*!
             * \brief Indicates the current selection mode.
             * \details If `true`, the attack selection dialog is printed. If `false`, the action selection dialog is printed.
             */
            bool attackChoice = false;

            sf::Text attacks[4];
            /*!
             * \brief The number of PP of an attack.
             */
            sf::Text ppTxt;
            /*!
             * \brief Prints "PP:" next to Battle::ppTxt
             */
            sf::Text ppStrTxt;
            /*!
             * \brief Prints the type of the attack.
             */
            sf::Sprite type;

            /*!
             * \brief The little arrow at the bottom of the dialog box.
             */
            sf::Sprite dialogArrow;

            /*!
             * \brief The current phase of the battle.
             *
             * During the battle, two phases alternate:
             * - Action selection (Menu with different actions)
             * - Turn action (Attack, item and other actions)
             * When the turn is launched, the variable BattleCtrl::turnActivated is set to true.
             * When the turn is over, the variable BattleCtrl::turnActivated is set to false.
             * Battle have its own variable, allowing to know when the variable has changed.
             */
            int phase = 0;

            Dialog *dialog = nullptr;

            Model::BattleData &data;

            bool dialogOver = true;

          public:
            /*!
             * \brief Initialises a battle.
             * \param atkTeam The player's team.
             * \param defTeam The opponent's team.
             * \param trainerClass The trainer's class.
             * \param background The background of the battle.
             * \param data The data used for the battle.
             */
            Battle(const Model::OpTeam *atkTeam, const Model::OpTeam *defTeam, std::string trainerClass, std::string background, Model::BattleData &data);

            /*!
             * \brief One frame of the battle.
             * \param frame The frame of the game.
             * \param atk The player's turn data.
             * \param def The opponent's turn data.
             * \param actionQueue A queue of the actions to do.
             * \param turnActivated A pointer to BattleCtrl::turnActivated. See Battle::phase for further explanation.
             * \param atkFirst If `true`, the player's turn is the first. Else, the opponent's turn is the first.
             */
            GameStatus operator()(sf::RenderTexture &frame, Model::TurnData const &atk, Model::TurnData const &def, std::queue<Model::TurnAction> &actionQueue, bool *turnActivated, bool atkFirst);
            /*!
             * \brief Moves the cursor.
             * \param where In which direction the cursor has to move.
             */
            void moveCur(Model::Side where);

            int getCurPos() { return curPos.getValue(); }

            /*!
             * \brief Tooggle the interface printed, the action or attack selection.
             */
            void toggleAttackChoice();

            int getPhase() { return phase; }

            bool isAttackChoice() { return attackChoice; }

            void passDialog() {
                if(dialog != nullptr)
                    dialog->pass();
            }
        };

    } // namespace View
} // namespace OpMon
