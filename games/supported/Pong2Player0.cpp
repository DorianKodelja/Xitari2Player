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
#include "Pong2Player0.hpp"
#include "ale_interface.hpp"
#include "../RomUtils.hpp"
#include <iostream>

using namespace ale;


Pong2Player0Settings::Pong2Player0Settings() {
    reset();
}


/* create a new instance of the rom */
RomSettings* Pong2Player0Settings::clone() const { 
    
    RomSettings* rval = new Pong2Player0Settings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void Pong2Player0Settings::step(const System& system) {
    int left = readRam(&system, 13); // left player score
    int right = readRam(&system, 14); // right player score
    // The RL score is the difference in scores
    int newScore = right-left;
    int newScoreB = left-right;
    points=left+right;
    m_reward = newScore - m_score;
    m_rewardB = newScoreB - m_scoreB;
    m_score = newScore;
    m_scoreB = newScoreB;
    sideBouncing=readRam(&system, 0x91);
    wallBouncing=readRam(&system, 0x94)==128;
    crash=readRam(&system, 0x90)==0;
    serving=readRam(&system, 0xB6)==0;
    if (m_reward==-1) {m_rewardB=0;}
    else {m_reward=0;}  

    if(m_reward!=0){
    }
    // The game ends when we reach 21 points
    m_terminal = (left == 21 || right == 21); 
}


/* is end of game */
bool Pong2Player0Settings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t Pong2Player0Settings::getReward() const { 

    if(m_reward!=0){
    //std::cout<< "\n\n\n-----------------PongGetRewrd rewardA  "<<m_reward << "rewardB" << m_rewardB <<"--------------"<<std::endl;
    }
    return m_reward; 
}
/* get the most recently observed reward */
reward_t Pong2Player0Settings::getRewardB() const { 

    return m_rewardB; 
}
double Pong2Player0Settings::getSideBouncing() const { 

    return sideBouncing; 
}



bool Pong2Player0Settings::getWallBouncing() const { 

    return wallBouncing; 
}

int Pong2Player0Settings::getPoints() const { 

    return points; 
}
bool Pong2Player0Settings::getCrash() const { 

    return crash; 
}
bool Pong2Player0Settings::getServing() const { 

    return serving; 
}

bool Pong2Player0Settings::isLegal(const Action& a) const {
    switch (a) {
        // right player
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
            return true;
        default:
            return false;
    }   
}

bool Pong2Player0Settings::isLegalB(const Action& a) const {
    switch (a) {
        // left player
       case PLAYER_B_NOOP:
       case PLAYER_B_FIRE:
       case PLAYER_B_RIGHT:
       case PLAYER_B_LEFT:
            return true;
        default:
            return false;
    }   
}
/* is an action part of the minimal set? */
bool Pong2Player0Settings::isMinimal(const Action &a) const {
    return true; // all legal actions are minimal
}
bool Pong2Player0Settings::isMinimalB(const Action &a) const {
               return true; 
}

/* reset the state of the game */
void Pong2Player0Settings::reset() {
    
    m_reward   = 0;
    m_rewardB  = 0;
    m_score    = 0;
    m_scoreB   = 0;
    m_terminal = false;
}

        
/* saves the state of the rom settings */
void Pong2Player0Settings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_rewardB);
  ser.putInt(m_scoreB);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void Pong2Player0Settings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_rewardB = ser.getInt();
  m_scoreB = ser.getInt();
  m_terminal = ser.getBool();
}

ActionVect Pong2Player0Settings::getStartingActions() {

    ActionVect startingActions;
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(SELECT);
    startingActions.push_back(PLAYER_A_NOOP);
    startingActions.push_back(SELECT);
    startingActions.push_back(RESET);
    return startingActions;

}
