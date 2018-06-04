import { Component, OnInit } from '@angular/core';
import { NavController } from 'ionic-angular';
import { AuthenticationService, DeviceService } from '../../services/index'
import { LoginPage } from '../login/login';

import { TabsPage } from '../tabs/tabs';
import { ConfigPage  } from '../config/config';
import { LoadingController } from 'ionic-angular';
import { Device } from '../../models/index';
declare type MyHandler = (myArgument: string) => void;

import {mockDevices} from '../../helpers/mock-data';


@Component({
  selector: 'page-home',
  templateUrl: 'home.html',
  providers: [AuthenticationService]
})
export class HomePage implements OnInit {



  handler: MyHandler;
  devices:Device[]=[];
  refresh:boolean=false;

  constructor(public navCtrl: NavController, public authService: AuthenticationService,public devService: DeviceService,public loadingCtrl: LoadingController) {
    //this.devices=[] as Device[];
    this.handler= (_params) => {
     return new Promise((resolve, reject) => {
         console.log("a refrescar!!!");
        this.refreshDevices();
         resolve();
     });}
  }


  refreshDevices()
  {
      let loader = this.loadingCtrl.create({
      content: "Please wait...",
    });
    loader.present();

    this.devService.getDevices('1').subscribe
    (
      (value) =>
      {
        this.devices=value;
        console.log(this.devices);
      },
      (error) => { loader.dismiss();},
      ()=> { loader.dismiss();}
    );


  }

  ngOnInit()
  {
    this.refreshDevices();
  }

  logout()
  {
    this.authService.logout();
    this.navCtrl.push(LoginPage);
  }

  addDevice()
  {
      let device:Device=new Device();
      console.log("add device");

      console.log("crear device test");
      Object.assign(device,mockDevices[0]);
      console.log(device);
      device.name="test";
      device.imei=Math.random().toString(36);
    //Le pasamos en el push una callback function para que sea llamada cuando necesitemos refrescar
    //la lista de dispositivos
    this.navCtrl.push(ConfigPage, { device: device, callback:this.handler });
  }


  getEnUso(): Device[] {   return this.devices.filter(e=>  e.salidas.length>0 );  }

  isEnUso():boolean  {    return (this.getEnUso().length>0);  }

  getEnParo(): Device[] {   return (this.devices.filter(e=>  e.salidas.length==0 ));  }

  isEnParo():boolean  {    return (this.getEnParo().length>0);  }



  clickedDevice(evt:Event,device:Device)
  {
    this.navCtrl.push(TabsPage,{ device:device});
  }

   ionViewCanEnter() {
    let auth= this.authService.isAuthenticated();
    if (!auth)
    {
     console.log("Enviando..->");
     // Let's navigate from TabsPage to Page1
      this.navCtrl.push(LoginPage);
    }

    return auth;
  }

}
