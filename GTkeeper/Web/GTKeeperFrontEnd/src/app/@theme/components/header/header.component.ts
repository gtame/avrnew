import { Component, Input, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { NbMenuService, NbSidebarService } from '@nebular/theme';
import { UserService } from '../../../@core/data/users.service';
import { AnalyticsService } from '../../../@core/utils/analytics.service';

import { filter } from 'rxjs/operators';

import { NbAuthJWTToken, NbAuthService } from '@nebular/auth';


@Component({
  selector: 'ngx-header',
  styleUrls: ['./header.component.scss'],
  templateUrl: './header.component.html',
})
export class HeaderComponent implements OnInit {


  @Input() position = 'normal';

  user= {};
  username : string;

  userMenu = [{ title: 'Profile' }, { title: 'Log out', data:{ action: 'logout'} }, { title: 'otras heces' }];

  constructor(private sidebarService: NbSidebarService,
              private menuService: NbMenuService,
              private userService: UserService,
              private analyticsService: AnalyticsService,
              private authService: NbAuthService,
              private routerService: Router
            ) {

                this.authService.getToken().subscribe((token:NbAuthJWTToken) => { 
                  if (token.isValid()) {
                 this.user = token.getPayload()['user'];
                  }
                 //this.username= this.user.UserName;
                });

                this.authService.onTokenChange()
                .subscribe((token: NbAuthJWTToken) => {
                  if (token.isValid()) {
                    this.user = token.getPayload()['user']; // here we receive a payload from the token and assigne it to our `user` variable 
                    //this.userMenu.push({ title: this.user.UserName});
                    console.log(this.user);
                  }
          
                });
                

                this.menuService.onItemClick()
                  .pipe(filter(({ tag }) => tag === 'my-context-menu'))
                 .subscribe((event: {tag: string, item: any}) => { console.log(event.item);

                  if (this.authService.isAuthenticated )
                  {
                      if (event.item.data.action=='logout')
                      {
                    
                        this.authService.logout('email').subscribe(function (result) {
                            var redirect = result.getRedirect();
                            if (redirect) {
                              routerService.navigateByUrl('/auth/login');
                              return true;
                              /*
                              setTimeout(function () {
                                    return routerService.navigateByUrl(redirect);
                                }, _this.redirectDelay);*/
                            }
                        });
                      
                      }   
                  }
        
        }

  }

  ngOnInit() {

   
    
    /*
    this.userService.getUsers()
      .subscribe((users: any) => this.user = users.nick);*/
  }

 

  toggleSidebar(): boolean {
    this.sidebarService.toggle(true, 'menu-sidebar');
    return false;
  }

  toggleSettings(): boolean {
    this.sidebarService.toggle(false, 'settings-sidebar');
    return false;
  }

  goToHome() {
    this.menuService.navigateHome();
    
  }

  startSearch() {
    this.analyticsService.trackEvent('startSearch');
  }
}
