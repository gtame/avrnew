import { Component } from '@angular/core';
import { MenuController, NavController ,NavParams} from 'ionic-angular'
import { Device } from '../../models/index';

import { HomePage } from '../home/home';
import { App } from 'ionic-angular';

@Component({
  selector: 'page-estadisticas',
  templateUrl: 'estadisticas.html'
})
export class EstadisticasPage {

  public device:Device;

  constructor(public menu: MenuController,private params:NavParams,private app: App,public navCtrl: NavController) {
      this.device=params.data;
  }

  public backToRoot() {
        // navigates back to the root of the stack,
        // which based on the model above is tab1Root
        let nav= this.app.getRootNav();
     //   nav.push(HomePage);
        nav.popToRoot();
    }


      ionViewDidEnter() {
        
        this.menu.enable(false, 'mnuconfig');
        this.menu.enable(false, 'mnuprog');
        this.menu.enable(true, 'mnustat');
      }

}
