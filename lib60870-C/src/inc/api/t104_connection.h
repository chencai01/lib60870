/*
 *  Copyright 2016 MZ Automation GmbH
 *
 *  This file is part of lib60870-C
 *
 *  lib60870-C is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  lib60870-C is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with lib60870-C.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#ifndef SRC_INC_T104_CONNECTION_H_
#define SRC_INC_T104_CONNECTION_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sT104Connection* T104Connection;

T104Connection
T104Connection_create(const char* hostname, int tcpPort);

/**
 * \brief Set connection parameters.
 *
 * If not set the default parameters are used. This function must be called before the
 * T104Connection_connect function is called! If the function is called after the connect
 * the behavior is undefined.
 *
 */
void
T104Connection_setConnectionParameters(T104Connection self, T104ConnectionParameters parameters);

T104ConnectionParameters
T104Connection_getConnectionParameters(T104Connection self);

/**
 * \brief Sets the timeout for connecting to the server (in ms)
 *
 * \param self
 * \param millies timeout value in ms
 */
void
T104Connection_setConnectTimeout(T104Connection self, int millies);

/**
 * \brief non-blocking connect.
 *
 * Invokes a connection establishment to the server and returns immediately.
 *
 * \param self
 */
void
T104Connection_connectAsync(T104Connection self);

/**
 * \brief blocking connect
 *
 * Establishes a connection to a server. This function is blocking and will return
 * after the connection is established or the connect timeout elapsed.
 *
 * \return true when connected, false otherwise
 */
bool
T104Connection_connect(T104Connection self);

/**
 * \brief start data transmission on this connection
 *
 * After issuing this command the client (master) will receive spontaneous
 * (unsolicited) messages from the server (slave).
 */
void
T104Connection_sendStartDT(T104Connection self);

void
T104Connection_sendInterrogationCommand(T104Connection self, CauseOfTransmission cot, int ca, uint8_t qoi);

void
T104Connection_sendCounterInterrogationCommand(T104Connection self, CauseOfTransmission cot, int ca, uint8_t qcc);

/**
 * \brief  Sends a read command (C_RD_NA_1 typeID: 102)
 *
 * This will send a read command C_RC_NA_1 (102) to the slave/outstation. The COT is always REQUEST (5).
 * It is used to implement the cyclical polling of data application function.
 *
 * \param ca Common address of the slave/server
 * \param ioa Information object address of the data point to read
 */
void
T104Connection_sendReadCommend(T104Connection self, int ca, int ioa);

/**
 * \brief Sends a clock synchronization command (C_CS_NA_1 typeID: 103)
 *
 * \param ca Common address of the slave/server
 * \param time new system time for the slave/server
 */
void
T104Connection_sendClockSyncCommand(T104Connection self, int ca, CP56Time2a time);

/**
 * \brief Send a test command (C_TS_NA_1 typeID: 104)
 *
 * Note: This command is not supported by IEC 60870-5-104
 *
 * \param ca Common address of the slave/server
 */
void
T104Connection_sendTestCommand(T104Connection self, int ca);

void
T104Connection_sendControlCommand(T104Connection self, TypeID typeId, CauseOfTransmission cot,
        int ca, InformationObject command);

typedef bool (*ASDUReceivedHandler) (void* parameter, ASDU asdu);

void
T104Connection_setASDUReceivedHandler(T104Connection self, ASDUReceivedHandler handler, void* parameter);

/**
 * \brief Close the connection and free all related resources
 */
void
T104Connection_destroy(T104Connection self);

#ifdef __cplusplus
}
#endif

#endif /* SRC_INC_T104_CONNECTION_H_ */
