import { Component } from '@angular/core';
import { NavController ,NavParams} from 'ionic-angular'

import { Device } from '../../models/index';

import { HomePage } from '../home/home';
import { App } from 'ionic-angular';

@Component({
  selector: 'page-about',
  templateUrl: 'about.html'
})
export class AboutPage {

  public device:Device;

  constructor(private params:NavParams,private app: App,public navCtrl: NavController) {
      this.device=params.data;
  }

  public backToRoot() {
        // navigates back to the root of the stack,
        // which based on the model above is tab1Root
        let nav= this.app.getRootNav();
     //   nav.push(HomePage);
        nav.popToRoot();
    }

}
