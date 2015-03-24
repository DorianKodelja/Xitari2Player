/* *****************************************************************************
 * Xitari
 *
 * Copyright 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Breakout2Player.hpp"

#include "../RomUtils.hpp"


using namespace ale;


Breakout2PlayerSettings::Breakout2PlayerSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* Breakout2PlayerSettings::clone() const {

    RomSettings* rval = new Breakout2PlayerSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void Breakout2PlayerSettings::step(const System& system) {

    // update the reward
    int x = readRam(&system, 77);
    int y = readRam(&system, 76);
    reward_t player_b_score = 1 * (x & 0x000F) + 10 * ((x & 0x00F0) >> 4) + 100 * (y & 0x000F);

    x = readRam(&system, 78);
    y = readRam(&system, 79);
    reward_t player_a_score = 1 * (x & 0x000F) + 10 * ((x & 0x00F0) >> 4) + 100 * (y & 0x000F);

    m_reward = player_a_score + player_b_score - m_score;
    m_score = player_a_score + player_b_score;

    // update terminal status
    int byte_val = readRam(&system, 57);
    if (!m_started && byte_val == 5) m_started = true;
    m_terminal = m_started && byte_val == 0;
    m_lives = byte_val;
}


/* is end of game */
bool Breakout2PlayerSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t Breakout2PlayerSettings::getReward() const {

    return m_reward;
}

bool Breakout2PlayerSettings::isLegal(const Action& a) const {
    switch (a) {
        case PLAYER_A_NOOP:
        // player 1
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        // player 2
        case PLAYER_A_UP:
        case PLAYER_A_DOWN:
        case PLAYER_A_GRIP_BOOSTER:

        case PLAYER_B_NOOP:
        // player 3
        case PLAYER_B_FIRE:
        case PLAYER_B_RIGHT:
        case PLAYER_B_LEFT:
        // player 4
        case PLAYER_B_UP:
        case PLAYER_B_DOWN:
        case PLAYER_B_GRIP_BOOSTER:
            return true;
        default:
            return false;
    }   
}

/* is an action part of the minimal set? */
bool Breakout2PlayerSettings::isMinimal(const Action &a) const {
    return true; // all legal actions are minimal
}


/* reset the state of the game */
void Breakout2PlayerSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_lives    = 5;
    m_terminal = false;
    m_started  = false;
}


/* saves the state of the rom settings */
void Breakout2PlayerSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putBool(m_started);
  ser.putInt(m_lives);
}


/* loads the state of the rom settings */
void Breakout2PlayerSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_started = ser.getBool();
  m_lives = ser.getInt();
}

ActionVect Breakout2PlayerSettings::getStartingActions() {
    ActionVect startingActions;
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(SELECT);
    startingActions.push_back(RESET);
    return startingActions;
}

