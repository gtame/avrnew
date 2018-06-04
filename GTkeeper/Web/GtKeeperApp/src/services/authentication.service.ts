import { Injectable } from '@angular/core';
import { Http, Headers, Response,RequestOptions } from '@angular/http';
import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/map'

@Injectable()
export class AuthenticationService {
    constructor(private http: Http) { }

    login(username: string, password: string) {
        return this.http.post('/api/authenticate', JSON.stringify({ username: username, password: password }))
            .map((response: Response) => {
                // login successful if there's a jwt token in the response
                let user = response.json();
                if (user && user.token) {
                    // store user details and jwt token in local storage to keep user logged in between page refreshes
                    localStorage.setItem('currentUser', JSON.stringify(user));
                }
            });
    }


    remember(mail: string):Observable<Response> {
       
         // Parameters obj-
        let params: URLSearchParams = new URLSearchParams();
        params.append('mail',  mail);
        //Http request-
        return this.http.get('/api/remember?' + params.toString());
    /*
    .subscribe(
        (response) => this.onGetForecastResult(response.json()), 
        (error) => this.onGetForecastError(error.json()), 
        () => this.onGetForecastComplete()
        );
*/
 
    }


    logout() {
        // remove user from local storage to log user out
        localStorage.removeItem('currentUser');
    }


    isAuthenticated() : boolean
    {
        if (localStorage.getItem('currentUser')) {
            // logged in so return true
            return true;
        }

        // not logged in so redirect to login page with the return url
        //this.router.navigate(['/login'], { queryParams: { returnUrl: state.url }});
        return false;
    }
}