/**
 * @file
 * @author Rostislav Špinar <rostislav.spinar@microrisc.com>
 * @author Roman Ondráček <ondracek.roman@centrum.cz>
 * @version 1.0
 *
 * @section LICENSE
 * Copyright 2015 MICRORISC s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "IQRFTR.h"

/**
 * Reset TR module
 */
void IQRFTR::reset() {
	if (spi->getMasterStatus()) {
		this->turnOff();
		// RESET pause
		delay(100);
		this->turnOn();
		delay(1);
	} else {
		spi->setStatus(spi->statuses::BUSY);
	}
}

/**
 * Enter TR module into ON state
 */
void IQRFTR::turnOn() {
	pinMode(TR_RESET_IO, OUTPUT);
	digitalWrite(TR_RESET_IO, LOW);
}

/**
 * Enter TR module into OFF state
 */
void IQRFTR::turnOff() {
	pinMode(TR_RESET_IO, OUTPUT);
	digitalWrite(TR_RESET_IO, HIGH);
}