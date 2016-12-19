/*
 * profile_creator.h
 *
 *  Created on: 19.12.2016
 *      Author: abl395
 */

#ifndef PROFILE_CREATOR_H_
#define PROFILE_CREATOR_H_

class Profile_Creator {
    public:
		Profile_Creator(){}
        ~Profile_Creator(){}
        void create_profile(void);

    private:
        Profile_Creator(const Profile_Creator& l);
        Profile_Creator& operator=(Profile_Creator& l);
};

#endif /* PROFILE_CREATOR_H_ */
