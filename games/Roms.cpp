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
#include "Roms.hpp"
#include "RomSettings.hpp"
#include "RomUtils.hpp"
#include "emucore/OSystem.hxx"

// include the game implementations

#include "supported/Pong2Player.hpp"
#include "supported/Pong2PlayerVS.hpp"
#include "supported/Pong2Player0.hpp"
#include "supported/Pong2Player05.hpp"
#include "supported/Pong2Player025.hpp"
#include "supported/Pong2Player075.hpp"
#include "supported/Pong2Player05p.hpp"
#include "supported/Pong2Player025p.hpp"
#include "supported/Pong2Player075p.hpp"

using namespace ale;


/* list of supported games */
static const RomSettings *roms[]  = {
    new Pong2PlayerSettings(),
    new Pong2PlayerVSSettings(),
    new Pong2Player0Settings(),
    new Pong2Player05Settings(),
    new Pong2Player025Settings(),
    new Pong2Player075Settings(),
    new Pong2Player05pSettings(),
    new Pong2Player025pSettings(),
    new Pong2Player075pSettings(),
};


/* looks for the RL wrapper corresponding to a particular rom title */
RomSettings *ale::buildRomRLWrapper(const std::string &rom) {

    size_t slash_ind = rom.find_last_of("/\\");
    std::string rom_str = rom.substr(slash_ind + 1);
    size_t dot_idx = rom_str.find_first_of(".");
    rom_str = rom_str.substr(0, dot_idx);

    for (size_t i=0; i < sizeof(roms)/sizeof(roms[0]); i++) {
        if (rom_str == roms[i]->rom()) return roms[i]->clone();
    }

    return NULL;
}

void ale::applyRomSettings(RomSettings * settings, OSystem * osystem) {

    assert(settings != NULL && osystem != NULL);

    if (settings->swapPorts())
        osystem->console().toggleSwapPorts();
}
