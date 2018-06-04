import { Injectable } from '@angular/core';
import { Http, Headers, Response,RequestOptions } from '@angular/http';
import { Observable } from 'rxjs/Observable';

import { Device } from '../models/index';

import 'rxjs/add/operator/map'

@Injectable()
export class DeviceService {
    constructor(private http: Http) { }

    getDevices(username: string) {
        return this.http.get('/api/devices/'+username+'/',this.jwt()).map((response: Response) => response.json());
    }



    addDevice(device: Device) {
       return this.http.post('/api/devices/add',JSON.stringify( device),this.jwt()).map((response: Response) => response.json());

    }



    private jwt() {
        // create authorization header with jwt token
        let currentUser = JSON.parse(localStorage.getItem('currentUser'));
        if (currentUser && currentUser.token) {
            let headers = new Headers({ 'Authorization': 'Bearer ' + currentUser.token });
            return new RequestOptions({ headers: headers });
        }
    }
}
