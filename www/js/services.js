var server = 'http://192.168.1.105:8888';

angular.module('app.services', [])
        .factory('HttpGet', function ($http) {
            return {
                getLuces: function () {
                    // $http returns a promise, which has a then function, which also returns a promise.
                    var part = '/rock';
                    var url = server.concat(part);
                    console.log('Solicito: ', url)
                    return $http.get(url)
                            .then(function (response) {
                                // In the response, resp.data contains the result. Check the console to see all of the data returned.
                                console.log('Get mecer', response);
                                return response.data;
                            });
                },
                getPuertas: function () {
                    var part = '/puertas';
                    var url = server.concat(part);
                    console.log('Solicito: ', url);
                    return $http.get(url).then(function (response) {
                        console.log('Get puertas', response);
                        return response.data;
                    })
                },
		getProximidad: function () {
                    var part = '/proximidad';
                    var url = server.concat(part);
                    console.log('Solicito: ', url);
                    return $http.get(url).then(function (response) {
                        console.log('Get proximidad', response);
                        return response.data;
                    })
                },
                getFoto: function () {
                    var part = '/camara';
                    var url = server.concat(part);
                    console.log('Solicito: ', url);
                    return $http.get(url).then(function (response) {
                        console.log('Get foto', response);
                        return response.data;
                    });
                }

            };
        })

        .factory('HttpPost', function ($http) {
            return {
                login: function (info, callback, err) {
                    console.log("login");
                    return $http({
                        method: "POST",
                        url: server + '/usuarios/login',
                        data: {
                            "user": info.user,
                            "pass": info.pass
                        }
                    }).success(callback).error(err);
                }
			};
		})

         .factory('HttpPost1', function ($http) {
            return {  
                registro:function (info, callback, err) {
                    console.log("NEW");
                    return $http({
                        method: "POST",
                        url: server + '/usuarios/nuevo',
                        data: {
                            "user": info.user,
                            "pass": info.pass
                        }
                    }).success(callback).error(err);
                } }})
             
         .factory('HttpPost2', function ($http) {   
			return {  
                leds:function (info, callback, err) {
                    return $http({
                        method: "POST",
                        url: server + '/rock',
                        data: {
							"num": info.num,
							"valor": info.valor
                           
                        }
                    }).success(callback).error(err);
                }
            };
        });
