import { Component, Input, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { NbMenuService, NbSidebarService } from '@nebular/theme';
import { UserService } from '../../../@core/data/users.service';
import { AnalyticsService } from '../../../@core/utils/analytics.service';

import { filter } from 'rxjs/operators';

import { NbAuthJWTToken, NbAuthService } from '@nebular/auth';
import { debug } from 'util';


@Component({
  selector: 'ngx-header',
  styleUrls: ['./header.component.scss'],
  templateUrl: './header.component.html',
})
export class HeaderComponent implements OnInit {


  @Input() position = 'normal';

  user= {};
  username : string;

  userMenu = 
  [{ title: 'Profile',
    data:
    { 
      action: 'profile', 
      callback: () => {alert('profile');}
    }
  }, 
  { title: 'Log out',
     data:
     { 
      action: 'logout', 
      callback: () => 
      {
        debugger;
        this.authService.logout('email').subscribe( (result) => { 
              var redirect = result.getRedirect();
              if (redirect)
              this.routerService.navigateByUrl(redirect);
         });
      }
    }
  }];

    menuSubscription: any;

  constructor(private sidebarService: NbSidebarService,
              private menuService: NbMenuService, 
              private userService: UserService,
              private analyticsService: AnalyticsService,
              private authService: NbAuthService,
              private routerService: Router
            ) {

                //Token
                this.authService.getToken().subscribe((token:NbAuthJWTToken) => { 
                  if (token.isValid()) 
                 this.user = token.getPayload()['user'];
                });

                this.authService.onTokenChange()
                .subscribe((token: NbAuthJWTToken) => {
                  if (token.isValid()) 
                    this.user = token.getPayload()['user'];         
                });

                //Menu events       
                this.menuSubscription=this.menuService.onItemClick()
                .pipe(filter(({ tag }) => tag === 'my-context-menu'))
                .subscribe((event: {tag: string, item: any}) => 
                { event.item.data.callback();  });
          }

  ngOnDestroy()
  {
    debugger;
    this.menuSubscription.unsubscribe();
    console.log('destroy');
  }

  ngOnInit() {

   
    
    /*
    this.userService.getUsers()
      .subscribe((users: any) => this.user = users.nick);*/
  }

  logout()
  {
    this.authService.logout('email').subscribe( (result) => { 
      var redirect = result.getRedirect();
      if (redirect)
        this.routerService.navigateByUrl(redirect);
    });
    
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
