import { Http, BaseRequestOptions, Response, ResponseOptions, RequestMethod, URLSearchParams,RequestOptions  } from '@angular/http';
import { MockBackend, MockConnection } from '@angular/http/testing';
import { User,Device } from '../models/index';
import {mockUsers,mockDevices} from './mock-data';

export let fakeBackendProvider = {
    // use fake backend in place of Http service for backend-less development
    provide: Http,
    useFactory: (backend: MockBackend, options: RequestOptions) => {
        // array in local storage for registered users
        let users: any[] = JSON.parse(localStorage.getItem('users')) || [];
        let devices: Device[]=new Array();


        //fake user --added
        mockUsers.forEach(e => {
          users.push(e);
        });



        //fake devices --added
        let storagedevices:any[] = JSON.parse(localStorage.getItem('devices')) || [];
        storagedevices.forEach( e => {
              devices.push(new Device(e));
        });

        if (storagedevices.length==0)
        {
        mockDevices.forEach(e => {
          let newDevice:Device;
          let found:number=devices.filter(dev=>{e.imei==dev.imei}).length;
          console.log(`found`  + found);
          if (found==0)
          {
            newDevice=new Device(e);
            devices.push(newDevice);
          }
        });
        }



        // configure fake backend
        backend.connections.subscribe((connection: MockConnection) => {
            // wrap in timeout to simulate server api call
            setTimeout(() => {



                // authenticate
                if (connection.request.url.endsWith('/api/authenticate') && connection.request.method === RequestMethod.Post) {
                    // get parameters from post request
                    let params = JSON.parse(connection.request.getBody());

                    // find if any user matches login credentials
                    let filteredUsers = users.filter(user => {
                        return user.username === params.username && user.password === params.password;
                    });

                    if (filteredUsers.length) {
                        // if login details are valid return 200 OK with user details and fake jwt token
                        let user = filteredUsers[0];
                        connection.mockRespond(new Response(new ResponseOptions({
                            status: 200,
                            body: {
                                id: user.id,
                                username: user.username,
                                firstName: user.firstName,
                                lastName: user.lastName,
                                token: 'fake-jwt-token'
                            }
                        })));
                    } else {
                        // else return 400 bad request
                        connection.mockError(new Error('Username or password is incorrect'));
                    }
                }



                //remember account
               else  if (connection.request.url.indexOf('/api/remember?')!=-1 && connection.request.method === RequestMethod.Get) {

                        let searchIndex=connection.request.url.indexOf('?');
                        console.log('remember');
                        let urlParams = new URLSearchParams(connection.request.url.substring(searchIndex+1));
                        let mail=urlParams.get('mail');
                        if (mail=='error')
                            connection.mockRespond(new Response(new ResponseOptions({ status: 401, statusText:'El mail no existe' })));
                        else
                            connection.mockRespond(new Response(new ResponseOptions({ status: 200 })));

                }

                // get users
                else if (connection.request.url.endsWith('/api/users') && connection.request.method === RequestMethod.Get) {
                    // check for fake auth token in header and return users if valid, this security is implemented server side in a real application
                    if (connection.request.headers.get('Authorization') === 'Bearer fake-jwt-token') {
                        connection.mockRespond(new Response(new ResponseOptions({ status: 200, body: users })));
                    } else {
                        // return 401 not authorised if token is null or invalid
                        connection.mockRespond(new Response(new ResponseOptions({ status: 401 })));
                    }
                }

                // get user by id
               else  if (connection.request.url.match(/\/api\/users\/\d+$/) && connection.request.method === RequestMethod.Get) {
                    // check for fake auth token in header and return user if valid, this security is implemented server side in a real application
                    if (connection.request.headers.get('Authorization') === 'Bearer fake-jwt-token') {
                        // find user by id in users array
                        let urlParts = connection.request.url.split('/');
                        let id = parseInt(urlParts[urlParts.length - 1]);
                        let matchedUsers = users.filter(user => { return user.id === id; });
                        let user = matchedUsers.length ? matchedUsers[0] : null;

                        // respond 200 OK with user
                        connection.mockRespond(new Response(new ResponseOptions({ status: 200, body: user })));
                    } else {
                        // return 401 not authorised if token is null or invalid
                        connection.mockRespond(new Response(new ResponseOptions({ status: 401 })));
                    }
                }

                // create user
               else  if (connection.request.url.endsWith('/api/users') && connection.request.method === RequestMethod.Post) {
                    // get new user object from post body
                    let newUser = JSON.parse(connection.request.getBody());

                    // validation
                    let duplicateUser = users.filter(user => { return user.username === newUser.username; }).length;
                    if (duplicateUser) {
                        return connection.mockError(new Error('Username "' + newUser.username + '" is already taken'));
                    }

                    // save new user
                    newUser.id = users.length + 1;
                    users.push(newUser);
                    localStorage.setItem('users', JSON.stringify(users));

                    // respond 200 OK
                    connection.mockRespond(new Response(new ResponseOptions({ status: 200 })));
                }

                // delete user
                else if (connection.request.url.match(/\/api\/users\/\d+$/) && connection.request.method === RequestMethod.Delete) {
                    // check for fake auth token in header and return user if valid, this security is implemented server side in a real application
                    if (connection.request.headers.get('Authorization') === 'Bearer fake-jwt-token') {
                        // find user by id in users array
                        let urlParts = connection.request.url.split('/');
                        let id = parseInt(urlParts[urlParts.length - 1]);
                        for (let i = 0; i < users.length; i++) {
                            let user = users[i];
                            if (user.id === id) {
                                // delete user
                                users.splice(i, 1);
                                localStorage.setItem('users', JSON.stringify(users));
                                break;
                            }
                        }

                        // respond 200 OK
                        connection.mockRespond(new Response(new ResponseOptions({ status: 200 })));
                    } else {
                        // return 401 not authorised if token is null or invalid
                        connection.mockRespond(new Response(new ResponseOptions({ status: 401 })));
                    }
                }




                // get devices
                else  if (connection.request.url.match(/\/api\/devices\/\S+$/) && connection.request.method === RequestMethod.Get) {
                    // check for fake auth token in header and return users if valid, this security is implemented server side in a real application
                    if (connection.request.headers.get('Authorization') === 'Bearer fake-jwt-token') {
                        connection.mockRespond(new Response(new ResponseOptions({ status: 200, body: devices })));
                    } else {
                        // return 401 not authorised if token is null or invalid
                        connection.mockRespond(new Response(new ResponseOptions({ status: 401 })));
                    }
                }



                   // create user
                else if (connection.request.url.endsWith('/api/devices/add') && connection.request.method === RequestMethod.Post) {

                    // get new user object from post body
                    let newDevice:Device;
                    newDevice=new Device(JSON.parse(connection.request.getBody()));

                    // validation
                    let duplicateDevice:number = devices.filter(device => { return device.imei == newDevice.imei; }).length;
                    if (duplicateDevice>0) {
                        return connection.mockError(new Error('Device "' + newDevice.imei + '" is already taken'));
                    }

                    // save new user
                    newDevice.id = (users.length + 1).toString();
                    devices.push(newDevice);
                    localStorage.setItem('devices', JSON.stringify(devices));

                    // respond 200 OK
                    connection.mockRespond(new Response(new ResponseOptions({ status: 200, body:newDevice })));
                }


            }, 500);

        });

        return new Http(backend, options);
    },
    deps: [MockBackend, BaseRequestOptions]
};
