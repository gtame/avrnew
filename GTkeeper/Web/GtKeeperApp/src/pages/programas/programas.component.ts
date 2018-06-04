import { Component, Input, OnInit } from '@angular/core';
import { MenuController, NavController ,NavParams} from 'ionic-angular'
import { Device,Programa,DiasSemana } from '../../models/index';
import { App } from 'ionic-angular';
import {Validators, FormBuilder, FormGroup } from '@angular/forms';

@Component({
  selector: 'programa-component',
  templateUrl: 'programa.component.html'
})
export class ProgramaComponent implements OnInit {

  @Input() programa:Programa;
  @Input() device:Device;

  public rootPage:any;
  private programaForm : FormGroup;
  
  DiasSemana: typeof DiasSemana = DiasSemana;

  constructor(public menu: MenuController,private params:NavParams,private app: App,public navCtrl: NavController, private formBuilder: FormBuilder ) {
      console.log(params.data);
      this.programa=params.get('programa');
      this.device=params.get('device');
       
  }

  ngOnInit()
  {

      let initDate: Date= new Date(1900,1);
      this.programaForm = this.formBuilder.group({
      name: [this.device.name, [Validators.required, Validators.minLength(4)]],
      phone: [this.device.phone, [Validators.required, Validators.minLength(9)]],
      pass: [this.device.pass, [Validators.required, Validators.minLength(4)]],
      sector: [null, [Validators.required]],
      motor: [this.device.motor,[]] ,
      tiempo: [initDate.toISOString(),[]],
      abono: [initDate.toISOString(),[]],
      previo: [,[]],
      diasArray:[this.programa.getDias(),[]],

    });

  }

  getSectores():any[]
  {
    return new Array(this.device.sectores);
  }


  getProgramas():any[]
  {
    return new Array(this.device.programas.length);
  }

  savePrograma(){
    console.log(this.programaForm.value);
  }


  public backToRoot() {
        // navigates back to the root of the stack,
        // which based on the model above is tab1Root
        let nav= this.app.getRootNav();
     //   nav.push(HomePage);
        nav.popToRoot();
    }


}
