import { Component, Input,Output,EventEmitter,OnInit  } from '@angular/core';
import { MenuController, NavController ,NavParams,AlertController} from 'ionic-angular'
import { Device,SmsAviso } from '../../models/index';
import {FormBuilder, FormGroup, Validators} from '@angular/forms';
import { App } from 'ionic-angular';
import { ValidationService } from './validation.service';



@Component({
  selector: 'config-component',
  templateUrl: 'config.component.html'
})
export class ConfigComponent implements OnInit {

  @Input() device:Device;
  @Output() save: EventEmitter<Device> = new EventEmitter();
  configForm: FormGroup;

  showAlertMessage:boolean;
   SmsAviso: typeof SmsAviso = SmsAviso;

  constructor(private alertCtrl:AlertController, public menu: MenuController,private params:NavParams,private app: App,public navCtrl: NavController,private formBuilder: FormBuilder)
  {

      this.showAlertMessage=true;
  }

  ngOnInit()
  {
      this.configForm = this.formBuilder.group({
      name: [this.device.name, [Validators.required, Validators.minLength(4)]],
      phone: [this.device.phone, [Validators.required, Validators.minLength(9)]],
      mail: [this.device.mail, [Validators.required, ValidationService.emailValidator]],
      pass: [this.device.pass, [Validators.required, Validators.minLength(4)]],
      sectores: [this.device.sectores, [Validators.required]],
      abono: [this.device.abono, [Validators.required]],
      motor: [this.device.motor,[]] ,
      avisoArray: [this.device.getAvisos(),[]]
    });
  }

    saveConfig() {

      this.device.setAvisos(this.configForm.value.avisoArray);

      if (this.configForm.dirty && this.configForm.valid)
      {

        console.log(this.configForm.value);
        Object.assign(this.device,this.configForm.value);
        console.log(this.device);

        console.log('lanzando');
        this.save.emit(this.device);
        console.log('lanzado');
      }

    }


    backToRoot() {
        // navigates back to the root of the stack,
        // which based on the model above is tab1Root
        let nav= this.app.getRootNav();
     //   nav.push(HomePage);
      if(this.ionViewCanLeave())
          nav.popToRoot();
    }


  ionViewCanLeave()
  {
    if (this.showAlertMessage &&  this.configForm.dirty)
    {
        let alertPopup = this.alertCtrl.create({
            title: 'Exit',
            message: '¿Are you sure?',
            buttons: [{
                    text: 'Exit',
                    handler: () => {
                        this.exitPage();
                    }
                },
                {
                    text: 'Stay',
                    handler: () => {
                        // need to do something if the user stays?
                        alertPopup.dismiss();
                        return false;
                    }
                }]
        });

              // Show the alert
      alertPopup.present();


        // Return false to avoid the page to be popped up
        return false;
    }
    else
      return true;
  }


    private exitPage() {
        this.showAlertMessage = false;
        this.navCtrl.pop();
    }


    ionViewDidEnter()
    {
      console.log("menu config");
      this.menu.enable(false, 'mnuprog');
      this.menu.enable(false, 'mnustat');
      this.menu.enable(true, 'mnuconfig');
    }

}
