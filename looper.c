/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <stdlib.h>
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define URI "http://shaji.in/plugins/eg-amp"

typedef enum {
	TOGGLE_RECORD   = 0,
    TOGGLE_PLAY = 1,
    TOGGLE_FILE = 2,
	INPUT  = 3,
	OUTPUT = 4
} PortIndex;

