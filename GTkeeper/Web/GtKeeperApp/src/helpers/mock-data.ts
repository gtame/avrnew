import { User,Device,SmsAviso } from '../models/index';

export let mockUsers:any [] = [
    { username: '1', password: '1' },
    { username: '2', password: '2' },
    { username: '3', password: '3' },
    { username: '4', password: '4' },
    { username: '5', password: '5' },
];

export let mockDevices: Device[]  = [

  new Device({ id: 'device1',  imei:'XXXXXx',  name: 'AguaSal',pass:'1234', phone: '653316799',mail:'gabrieltame@yahoo.es',sectores:15,abono:2,motor:true, description: 'aguasal',      enabled: true , avisos:SmsAviso.SMSFinSector | SmsAviso.SMSProgramacionWeb }),
  new Device( {id: 'device2',  imei:'XXXX1Xx',   name: 'AguaSal2',pass:'1234', phone: '653316799',mail:'gabrieltame@yahoo.es',sectores:15,abono:2,motor:true,    description: 'aguasal2',      enabled: true  , avisos:SmsAviso.SMSFinSector | SmsAviso.SMSProgramacionWeb}),

];

//avisos sms (SmsAviso.SMSFinSector | SmsAviso.SMSProgramacionWeb)  4+2;
