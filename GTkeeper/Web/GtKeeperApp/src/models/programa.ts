//Ej, 03101010120001200000
//03->Sector
//0101010-> Dias que se ejecutara (Martes,Jueves,Sabado)
//1200 -> Se ejecutara a las 12:00
//0120 -> Tiempo de riego
//0000 -> Tiempo de abono

import { EnumValues } from 'enum-values';


//Las enumeraciones no se pueden cargar como const, porque sino no se pueden utilizar en las templates (*.html)
//Sumatorio total 126 para todos los dias
export  enum DiasSemana {
	 NONE =0,
    L = (1 << 0),//1
    M= (1 << 1),//2
    X= (1 << 2),//4
    J= (1 << 3),//8
    V= (1 << 4),//16
    S= (1 << 5),//32
    D= (1 << 6)//64
}




export class Programa {

    id:number;
    sector:number;
    dias:DiasSemana;
    inicio: Date;
    riegoseg:number;
    abonoseg:number;



    getDias():number[]
    {
        let res:number[]=[];

        let values = EnumValues.getValues(DiasSemana);

         for (let valor of values)
         {
           if ((this.dias & valor)!=0)
              res.push(valor);
         }

         return res;
    }


     setDias(diasArray:number[])
    {
        this.dias=0;
        diasArray.forEach(e=> this.dias+=e);
    }


}
