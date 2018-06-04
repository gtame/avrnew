import { Component } from '@angular/core';
import { MenuController, NavController ,NavParams,AlertController} from 'ionic-angular'
import { Device } from '../../models/index';
import {FormBuilder, FormGroup, Validators} from '@angular/forms';
import { App } from 'ionic-angular';
import { DeviceService } from '../../services/index';
import {mockDevices} from '../../helpers/mock-data';
import 'rxjs/add/operator/catch';
import {Observable}              from 'rxjs/Observable';

@Component({
  selector: 'page-config',
  templateUrl: 'config.html'
})
export class ConfigPage {

  device:Device;
  callback:any;


  constructor(private alertCtrl:AlertController, public menu: MenuController,public devService: DeviceService,private params:NavParams,private app: App,public navCtrl: NavController,private formBuilder: FormBuilder)
  {
      this.callback = params.get("callback");
      this.device=params.get("device");

      //Si esta en modo edicion no tiene callback
      if (this.device==null)
        this.device=params.data;
/*
      console.log("device-config");
      console.log(this.device);*/
  }


  saveConfig(device:Device)
  {
    console.log(device);
    //Llamamos al servicio
    this.devService.addDevice(device).subscribe(
      data => { console.log(data);},
      err=> { console.log(err);},
      ()=> {console.log('complete!');}
    )
    //.catch( (e) => { console.log(e); return Observable.throw (e); });

    //refrescamos
    if (this.callback!=null)
        this.callback(true).then(()=>{
            this.navCtrl.pop();
        });

  }

   backToRoot() {
        // navigates back to the root of the stack,
        // which based on the model above is tab1Root
        let nav= this.app.getRootNav();
     //   nav.push(HomePage);
          nav.popToRoot();
    }

    ionViewDidEnter()
    {
      console.log("menu config");
      this.menu.enable(false, 'mnuprog');
      this.menu.enable(false, 'mnustat');
      this.menu.enable(true, 'mnuconfig');
    }

}
