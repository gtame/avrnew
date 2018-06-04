import { Component } from '@angular/core';
import { MenuController, NavController ,NavParams} from 'ionic-angular'
import { Device,Programa } from '../../models/index';
import { ProgramaComponent } from './programas.component';
import { App } from 'ionic-angular';
import {Validators, FormBuilder, FormGroup } from '@angular/forms';
@Component({
  selector: 'page-programas',
  templateUrl: 'programas.html'
})
export class ProgramasPage {

  public device:Device;
  public programas:Programa [];
  public tieneProgramas:boolean=false;
  public rootPage:any;

  constructor(public menu: MenuController,private params:NavParams,private app: App,public navCtrl: NavController, private formBuilder: FormBuilder ) {
      this.device=params.data;
      this.rootPage=ProgramasPage;
      this.tieneProgramas=(this.device.programas.length>0);
      this.programas=this.device.programas;
  }

  addPrograma()
  {
    this.navCtrl.push(ProgramaComponent,{ device: this.device, programa:new Programa()} )
  }



  public backToRoot() {
        // navigates back to the root of the stack,
        // which based on the model above is tab1Root
        let nav= this.app.getRootNav();
     //   nav.push(HomePage);
        nav.popToRoot();
    }


  ionViewDidEnter() {
        this.menu.enable(false, 'mnustat');
        this.menu.enable(false, 'mnuconfig');
        this.menu.enable(true, 'mnuprog');
      }
}
