import { NgModule, ErrorHandler } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { IonicApp, IonicModule, IonicErrorHandler } from 'ionic-angular';
import { MyApp } from './app.component';


import { FormsModule }    from '@angular/forms';
import { HttpModule } from '@angular/http';

// used to create fake backend
import { MockBackend, MockConnection } from '@angular/http/testing';
import { BaseRequestOptions } from '@angular/http';
import { fakeBackendProvider } from '../helpers/fake-backend';


import { HttpClientModule } from '@angular/common/http';
//import { AlertComponent } from '../directives/index';
import { AlertService, AuthenticationService, UserService,DeviceService} from '../services/index';

import { LoginPage } from '../pages/login/login';
import {EstadisticasPage} from '../pages/estadisticas/estadisticas'
import {ConfigPage} from '../pages/config/config';
import {ConfigComponent} from '../pages/config/config.component';
import { ProgramasPage } from '../pages/programas/programas';
import { ProgramaComponent } from '../pages/programas/programas.component'
import { AboutPage } from '../pages/about/about';
import { ContactPage } from '../pages/contact/contact';
import { HomePage } from '../pages/home/home';
import { TabsPage } from '../pages/tabs/tabs';

import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen';
import {ControlMessagesComponent} from '../pages/config/control-messages.component';

@NgModule({
  declarations: [
    MyApp,
    AboutPage,
    ContactPage,
    HomePage,
    TabsPage,
    LoginPage,
    ProgramasPage,
    EstadisticasPage,
    ConfigPage,
    ControlMessagesComponent,
    ConfigComponent,
    ProgramaComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule ,
    IonicModule.forRoot(MyApp),

    //Added modules
    FormsModule,
    
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    AboutPage,
    ContactPage,
    HomePage,
    TabsPage,
    ProgramasPage,
    EstadisticasPage,
    //Added components
    //AlertComponent,
    LoginPage,
    ConfigPage,
    ProgramaComponent
 //   RegisterComponent
  ],
  providers: [

    //Default Ionic providers
    StatusBar,
    SplashScreen,
    {provide: ErrorHandler, useClass: IonicErrorHandler},

    //Added providers

 //   AlertService,
    AuthenticationService,
    UserService,
    DeviceService,

    // providers used to create fake backend
    fakeBackendProvider,
    MockBackend,
    BaseRequestOptions
  ]
})
export class AppModule {}
