/*
 * gprs.h
 *
 *  Created on: 04/12/2019
 *      Author: Wendy
 */

#ifndef GPRS_H_
#define GPRS_H_

void verificacion_CREG(char *ram);
int verificar_IP(char *ram);
void verificar_GET(char *ram);
void verificar_GPRS(char *ram);
void iniciar_GPRS(char *ram, char *const apn);
void cerrar_GPRS(char *ram);
void metodo_GET(char *ram, char *respuesta, char *const url);

#endif /* GPRS_H_ */
