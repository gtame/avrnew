import { Component } from '@angular/core';
import { Device } from '../../models/index';
import { NavController ,NavParams} from 'ionic-angular'
import { AboutPage } from '../about/about';
import {EstadisticasPage} from '../estadisticas/estadisticas'
import { ProgramasPage } from '../programas/programas';
import { ConfigPage } from '../config/config';
@Component({
  templateUrl: 'tabs.html'
})
export class TabsPage {
  title:string;
  device: Device;

  tabProgramas= ProgramasPage;
  tabEstadisticas = EstadisticasPage;
  tabConfig = ConfigPage;

  constructor(public navCtrl: NavController,public params:NavParams) {
    this.title='asdasd';
    this.device=params.get('device');
    //¿?
    console.log(this.device);

  }
}
