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
	if (spi->isMasterEnabled()) {
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
 * TR module switch to programming mode
 */
void IQRFTR::enterProgramMode() {
	if (spi->isMasterEnabled()) {
		SPI.end();
		this->reset();
		pinMode(TR_SS_IO, OUTPUT);
		pinMode(TR_SDO_IO, OUTPUT);
		pinMode(TR_SDI_IO, INPUT);
		digitalWrite(TR_SS_IO, LOW);
		unsigned long enterMs = millis();
		do {
			// copy SDI to SDO for approx. 500ms => TR into prog. mode
			digitalWrite(TR_SDO_IO, digitalRead(TR_SDI_IO));
		} while ((millis() - enterMs) < (MILLI_SECOND / 2));
		digitalWrite(TR_SS_IO, HIGH);
		SPI.begin();
	} else {
		this->setControlStatus(controlStatuses::RESET);
		this->enableProgramFlag();
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

/**
 * Get TR control status
 * @return TR control status
 */
uint8_t IQRFTR::getControlStatus() {
	return this->controlStatus;
}

/**
 * Set TR control status
 * @param status TR control status
 */
void IQRFTR::setControlStatus(uint8_t status) {
	this->controlStatus = status;
}

/**
 * Enable programming flag
 */
void IQRFTR::enableProgramFlag() {
	this->programFlag = true;
}

/**
 * Disable programming flag
 */
void IQRFTR::disableProgramFlag() {
	this->programFlag = false;
}

/**
 * Get programming flag
 * @return Programming flag 
 */
bool IQRFTR::getProgramFlag() {
	return this->programFlag;
}

/**
 * Make TR module reset or switch to prog mode when SPI master is disabled
 */
void IQRFTR::controlTask() {
	static unsigned long timeoutMs;
	switch (this->getControlStatus()) {
		case controlStatuses::READY:
			spi->setStatus(spi->statuses::DISABLED);
			this->disableProgramFlag();
			break;
		case controlStatuses::RESET:
			spi->setStatus(spi->statuses::BUSY);
			SPI.end();
			this->turnOff();
			timeoutMs = millis();
			this->setControlStatus(controlStatuses::WAIT);
			break;
		case controlStatuses::WAIT:
			spi->setStatus(spi->statuses::BUSY);
			if (millis() - timeoutMs >= MILLI_SECOND / 3) {
				this->setControlStatus(controlStatuses::PROG_MODE);
			} else {
				digitalWrite(TR_SS_IO, HIGH);
				SPI.begin();
				this->setControlStatus(controlStatuses::READY);
			}
			break;
		case controlStatuses::PROG_MODE:
			SPI.end();
			this->reset();
			pinMode(TR_SS_IO, OUTPUT);
			pinMode(TR_SDI_IO, INPUT);
			pinMode(TR_SDO_IO, OUTPUT);
			digitalWrite(TR_SS_IO, LOW);
			timeoutMs = millis();
			do {
				// copy SDI to SDO for approx. 500ms => TR into prog. mode
				digitalWrite(TR_SDO_IO, digitalRead(TR_SDI_IO));
			} while ((millis() - timeoutMs) < (MILLI_SECOND / 2));
			digitalWrite(TR_SS_IO, HIGH);
			SPI.begin();
			this->setControlStatus(controlStatuses::READY);
			break;
	}
}