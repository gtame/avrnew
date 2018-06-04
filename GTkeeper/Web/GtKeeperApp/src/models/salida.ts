///Definimos el peso para que la ordenacion tenga efecto
export  enum TipoSalida{
	actNone=10,//No hay salida activa
	actPrograma=1,
	actSector=2,
	actAbono=3,
	actMotor=4,
	actLimpieza=5
} ;

/*
typedef struct {

  uint8_t Ident; //Sector(salida) ó Programa que lanzo
  TipoSalidaActiva Tipo; //Tipo, si es salida o
  uint8_t Sector; //Este campo es para asegurar cual era el sector cuando el programa se lanzo para evitar modificaciones entre que salta y se para
  time_t Desde; //Indica cuando se activo la salida ;)
  time_t Hasta;//Indica hasta cuando va a estar 'encendido el programa', si esta parado valor 0;

} SalidasActivas;

*/


export class Salida {

    id:number;
    programa:number;
    sector:number;
    tipo: TipoSalida;
    desde: Date;
    hasta: Date;
}