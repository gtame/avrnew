import { EnumValues } from 'enum-values';
import { Programa } from './programa';
import { Salida } from './salida';


//Las enumeraciones no se pueden cargar como const, porque sino no se pueden utilizar en las templates (*.html)
export  enum SmsAviso {
	SMSNone =0,//
	SMSInicioSector = (1 << 0),//1
	SMSFinSector= (1 << 1),//2
	SMSProgramacionWeb= (1 << 2),//4
	SMSReset= (1 << 3),//8
	SMSReservado1= (1 << 4),//16
	SMSReservado2= (1 << 5),//32
	SMSReservado3= (1 << 6)//64
}





export class Device {
    id: string;
    name: string;
    imei: string;
    phone: string;
    pass:string;
    avisos:SmsAviso;

    mail: string;

    sectores: number;
    abono: number;
    
    motor: boolean;

    description: string;

    lastUpdateProgram: Date;
    isProgramPendingSync: boolean;
    lastUpdateConfig: Date;
    isConfigPendingSync: boolean;
    lastCommitDate: Date;
    lastSync: Date;


    enabled: boolean;
    

    programas:Programa []=[];
    salidas:Salida []=[];

    public constructor(init?:Partial<Device>) {
        Object.assign(this, init);
    }

    getAvisos():number[]
    {
        let res:number[]=[];

        let values = EnumValues.getValues(SmsAviso);

         for (let valor of values)
         {
           if ((this.avisos & valor)!=0)
              res.push(valor);
         }

         return res;
    }


     setAvisos(avisoArray:number[])
    {
        this.avisos=0;
        avisoArray.forEach(e=> this.avisos+=e);
    }


}
