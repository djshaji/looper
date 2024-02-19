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

#define URI "http://shaji.in/plugins/looper"

/** Define a macro for converting a gain in dB to a coefficient. */
#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

typedef enum {
	TOGGLE_RECORD   = 0,
	TOGGLE_PLAY = 1,
	TOGGLE_FILE = 2,
	GAIN = 3,
	BUFFER_SIZE = 4,
    START = 5,
    END = 6,
	INPUT  = 7,
	OUTPUT = 8
} PortIndex;

typedef struct {
	// Port buffers
	float * gain;
    float * toggle_play ;
    float * toggle_rec ;
	float * input;
	float * output;
	float * buffer_size_control ;
	float * buffer ;
    float * start ;
    float * end ;
    int counter ;
    int buffer_size ;
    int recorded ;
} Looper;

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
	Looper * looper = (Looper*)malloc(sizeof(Looper));
	return (LV2_Handle)looper;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	Looper* looper = (Looper*)instance;

	switch ((PortIndex)port) {
	case GAIN:
		looper->gain = (const float*)data;
		break;
	case INPUT:
		looper->input = (const float*)data;
		break;
	case OUTPUT:
		looper->output = (float*)data;
		break;
    case TOGGLE_PLAY:
        looper -> toggle_play = (float *) data ;
        break ;
    case TOGGLE_RECORD:
        looper -> toggle_rec = (float *) data ;
        break ;
    case BUFFER_SIZE:
        looper -> buffer_size_control = (float *) data ;
        break ;
    case START:
        looper -> start = (float *) data ;
        break ;
    case END:
        looper -> end = (float *) data ;
        break ;
	}
}

static void
activate(LV2_Handle instance) {
    Looper * looper = (Looper *) instance ;
    * looper -> start = 0 ;
    * looper -> end = 0 ;
    looper -> counter = 0 ;
    looper -> buffer = malloc (* looper -> buffer_size_control * 1024) ;
    looper -> buffer_size = * looper -> buffer_size_control * 1024 ;
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
	const Looper * looper = (const Looper *)instance;

	const float        gain   = *(looper->gain);
	const float* const input  = looper->input;
	float* const       output = looper->output;
    const float coef = DB_CO(* looper -> gain);

    if (* looper -> toggle_rec > 0) {
        for (uint32_t pos = 0; pos < n_samples; pos++) {
            buffer [looper -> counter] = input [pos] ;
            looper -> counter ++ ;
            if (looper -> counter > looper -> buffer_size) {
                // * looper -> toggle_rec = 0 ;
                looper -> counter = 0 ;
                break ;
            }
        }
    } else if (* looper -> toggle_play > 0) {
        for (uint32_t pos = 0; pos < n_samples; pos++) {
            if (counter < (* looper -> start / 100) * looper -> buffer_size) {
                counter ++ ;
            } else if (counter > (* looper -> end / 100) * looper -> buffer_size) {
                counter ++ ;
            } else if (looper -> buffer[counter] == -1) {
                counter ++ ;
            } else {
                output[pos] = looper -> buffer [looper -> counter] * coef;
                looper -> counter ++ ;
            }

            if (looper -> counter > looper -> buffer_size {
                looper -> counter = 0 ;
            })
        }
    } else {
        if (looper -> counter > 0) {
            for (int i = looper -> counter ; i < looper -> buffer_size ; i ++) {
                looper -> buffer [i] = -1 ;
            }
        }

        looper -> counter = 0 ;
    }
}
