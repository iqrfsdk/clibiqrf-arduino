/**
 * @file
 * @author Rostislav Špinar <rostislav.spinar@microrisc.com>
 * @author Roman Ondráček <ondracek.roman@centrum.cz>
 * @version 1.1
 *
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

#include "IQRFCallbacks.h"

/**
 * Set Rx callback
 * @param callback Rx callback
 */
void IQRFCallbacks::setRxCallback(rxCallback_t callback) {
	this->rxCallback = callback;
}

/**
 * Call Rx callback
 */
void IQRFCallbacks::callRxCallback() {
	this->rxCallback();
}

/**
 * Set Tx callback
 * @param callback Tx callback
 */
void IQRFCallbacks::setTxCallback(txCallback_t callback) {
	this->txCallback = callback;
}

/**
 * Call Tx callback
 * @param packetId Packet ID
 * @param packetResult Packet result
 */
void IQRFCallbacks::callTxCallback(uint8_t packetId, uint8_t packetResult) {
	this->txCallback(packetId, packetResult);
}
