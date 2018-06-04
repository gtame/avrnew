import { Component } from '@angular/core';
import { Http } from '@angular/http';

import { NavController } from 'ionic-angular';
import { AlertController } from 'ionic-angular';
import { Platform } from 'ionic-angular';

import { HomePage } from '../home/home';
import { AuthenticationService } from '../../services/index'

@Component({
    templateUrl: 'login.html',
    providers: [AuthenticationService]
 //   styles: ['pages/login/login.css']
})
export class LoginPage {
    username:string;
    password:string;
    mail:string;
    remember:boolean;
    constructor(public nav: NavController, public platform: Platform, public http: Http, public authenticationApi: AuthenticationService,private alertCtrl: AlertController) {
    }

    loginDo() {

       this.authenticationApi.login(this.username, this.password).subscribe
       (
            (value) => {console.log("onNext " + value);this.nav.setRoot(HomePage);}, (error) => console.log("Error: " + error.message), () => console.log("Completed!")
        );
    }

    rememberDo()
    {

       this.authenticationApi.remember(this.mail).subscribe
       (
            (value) => {

                 if (value.status!=200)
                 {
                        let alert = this.alertCtrl.create({
                                    title: 'Error',
                                    subTitle: value.statusText,
                                    buttons: ['Ok']
                                });
                        alert.present();
                 }
                 else
                    this.nav.setRoot(HomePage);

                console.log("onNext " + value.status);

            }, (error) => console.log("Error: " + error.message), () => console.log("Completed!")
        );


    }
}
