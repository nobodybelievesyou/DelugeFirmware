/*
 * Copyright © 2014-2023 Synthstrom Audible Limited
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
*/

#include "gui/menu_item/menu_item_audio_clip_sample_marker_editor.h"
#include "processing/sound/sound.h"

#include "gui/menu_item/menu_item_sample_loop_point.h"
#include "definitions.h"
#include "gui/ui/sample_marker_editor.h"
#include "gui/ui/sound_editor.h"
#include "processing/source.h"
#include "storage/multi_range/multisample_range.h"
#include "model/sample/sample.h"
#include "io/uart/uart.h"
#include "hid/display/numeric_driver.h"
#include "gui/ui_timer_manager.h"
#include "gui/views/view.h"
#include "gui/ui/keyboard_screen.h"
#include "model/song/song.h"
#include "model/clip/audio_clip.h"

int MenuItemAudioClipSampleMarkerEditor::checkPermissionToBeginSession(Sound* sound, int whichThing,
                                                                       MultiRange** currentRange) {

	if (!isRelevant(sound, whichThing)) {
		return MENU_PERMISSION_NO;
	}

	// Before going ahead, make sure a Sample is loaded
	if (!((AudioClip*)currentSong->currentClip)->sampleHolder.audioFile) {
		return MENU_PERMISSION_NO;
	}

	return MENU_PERMISSION_YES;
}

void MenuItemAudioClipSampleMarkerEditor::beginSession(MenuItem* navigatedBackwardFrom) {

	soundEditor.shouldGoUpOneLevelOnBegin = true;
	sampleMarkerEditor.markerType = whichMarker;
	bool success = openUI(&sampleMarkerEditor); // Shouldn't be able to fail anymore
	if (!success) {
		uiTimerManager.unsetTimer(TIMER_SHORTCUT_BLINK);
	}
}