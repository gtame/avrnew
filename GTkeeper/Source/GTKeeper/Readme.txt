1 ESTRUCTURA DEL PROYECTO
===============================================
Se describe los directorios y el contenido del proyecto

--/			Codigo fuente(*.c/*.h/*.cpp)
--/Proteus	Simulaciones en proteus
--/Screens	Contiene los fuentes para las pantallas LC
--/Web		Ficheros que se suben al servidor web utilizados para realizar llamadas desde el dispositivo

2 CONFIGURACION DEL PROYECTO
===============================================

Modificado en HardwareSerial.h el tamaño de buffer de recepcion.
Cambiado de 64 a 128
Poner esta setting en el proyecto
#define SERIAL_RX_BUFFER_SIZE 128//Check 


PARA subir archivo al ftp desde filezilla, Transferencia->Tipo de transferencia->Binario sino luego no lo lee bien el fichero

AT+CLTS=1 // ESte comando es para que SIM900 sincronize la hora con la red GMS

revisar settings.h
Comprobar funcion now() sobre arduino, no estoy seguro si funciona igual simulado - no simulado. ahora va con callback funcion