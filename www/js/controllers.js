angular.module('app.controllers', ['ngResource'])

       .controller('movimientoCtrl', function($scope, $ionicLoading, HttpGet, HttpPost2) {
            $scope.movList=  [{text: "Estado Mecedor", checked: true}];

            $ionicLoading.show({
                template: 'Cargando...'
            });

             $scope.cambiar = function (valor) {
                if (valor === true){
                    return 1;
                }else{
                    return 0;
                }
            }

            /*$scope.request1 = function () {
                HttpPost2.leds({"num": "1", "valor": $scope.cambiar($scope.ledList[0].checked)}, function (data) {
                    console.log(data);
                }, function (err){console.log(err)});
                HttpPost2.leds({"num": "2", "valor": $scope.cambiar($scope.ledList[1].checked)}, function () {}, function (){});
                HttpPost2.leds({"num": "3", "valor": $scope.cambiar($scope.ledList[2].checked)}, function () {}, function (){});
                HttpPost2.leds({"num": "4", "valor": $scope.cambiar($scope.ledList[3].checked)}, function () {}, function (){});
                HttpPost2.leds({"num": "5", "valor": $scope.cambiar($scope.ledList[4].checked)}, function () {}, function (){});
            }
           

            HttpGet.getLuces().then(function(response) {
            $scope.luces = response;
            //console.log('luces ->',$scope.luces);

            if ($scope.luces.led1 === 1){
                $scope.ledList[0].checked= true;
            } else{
                $scope.ledList[0].checked= false;
            }
            if ($scope.luces.led2 === 1){
                $scope.ledList[1].checked= true;
            } else{
                $scope.ledList[1].checked= false;
            }
            if ($scope.luces.led3 === 1){
                $scope.ledList[2].checked= true;
            } else{
                $scope.ledList[2].checked= false;
            }
            if ($scope.luces.led4 === 1){
                $scope.ledList[3].checked= true;
            } else{
                $scope.ledList[3].checked= false;
            }
            if ($scope.luces.led5 === 1){
                $scope.ledList[4].checked= true;
            } else{
                $scope.ledList[4].checked= false;
            }
            $ionicLoading.hide();
            });*/
                      
	$ionicLoading.hide();
})

        .controller('puertasCtrl', function ($scope, $ionicLoading, HttpGet) {
            $scope.request = function () {
                HttpGet.getPuertas().then(function (response) {
                    $scope.puertas = response;
                    console.log('puertas ->', $scope.puertas);
                    $scope.doorList = [{text: "Puerta principal", icon: "icon ion-unlocked"},
                        {text: "Terraza", icon: "icon ion-locked"}, {text: "Puerta trasera", icon: "icon ion-locked"},
                        {text: "Garaje", icon: "icon ion-locked"}];
                    if ($scope.puertas.p1 === 0) {
                        $scope.doorList[0].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[0].icon = "icon ion-unlocked";
                    }
                    if ($scope.puertas.p2 === 0) {
                        $scope.doorList[1].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[1].icon = "icon ion-unlocked";
                    }
                    if ($scope.puertas.p3 === 0) {
                        $scope.doorList[2].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[2].icon = "icon ion-unlocked";
                    }
                    if ($scope.puertas.p4 === 0) {
                        $scope.doorList[3].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[3].icon = "icon ion-unlocked";
                    }
                    $ionicLoading.hide();
                });
            };
            $scope.request();
        })

	.controller('proxCtrl', function ($scope, $ionicLoading, HttpGet) {
            $scope.request = function () {
                HttpGet.getProximidad().then(function (response) {
                    $scope.proximidad = response;
                    console.log('proximidad ->', $scope.proximidad);
                    $scope.doorList = [{text: "Puerta principal", icon: "icon ion-unlocked"},
                        {text: "Terraza", icon: "icon ion-locked"}, {text: "Puerta trasera", icon: "icon ion-locked"},
                        {text: "Garaje", icon: "icon ion-locked"}];
                    if ($scope.puertas.p1 === 0) {
                        $scope.doorList[0].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[0].icon = "icon ion-unlocked";
                    }
                    if ($scope.puertas.p2 === 0) {
                        $scope.doorList[1].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[1].icon = "icon ion-unlocked";
                    }
                    if ($scope.puertas.p3 === 0) {
                        $scope.doorList[2].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[2].icon = "icon ion-unlocked";
                    }
                    if ($scope.puertas.p4 === 0) {
                        $scope.doorList[3].icon = "icon ion-locked";
                    } else {
                        $scope.doorList[3].icon = "icon ion-unlocked";
                    }
                    $ionicLoading.hide();
                });
            };
            $scope.request();
        })

	
        .controller('camaraCtrl', function ($scope, HttpGet) {

            //$scope.$on('clicked', function () { $ionicSideMenuDelegate.canDragContent(true) });
            $scope.request = function () {
                HttpGet.getFoto().then(function (response) {
                    $scope.pic = response;
                    console.log('Foto ->', $scope.pic);

                    document.getElementById("img").src = "data:image/png;base64, " + $scope.pic;

                });
            };
        })

        .controller('loginCtrl', ['$scope', '$state', '$stateParams', '$ionicPopup', '$timeout', '$ionicSideMenuDelegate', 'HttpPost', // The following is the constructor function for this page's controller. See https://docs.angularjs.org/guide/controller
            function ($scope, $state, $stateParams, $ionicPopup, $timeout, $ionicSideMenuDelegate, HttpPost) {
                $ionicSideMenuDelegate.canDragContent(false);
                $scope.user = {};
                $scope.loginForm = function (user) {
                    if (user.user && user.pass) {
                        console.log("Iniciando sesión");
                        $state.go("tabsController.rock");//BORRAR

                        HttpPost.login(user, function (respuesta) {
                            if (respuesta === true) {
                                $scope.showPopup = function () {
                                    var myPopup = $ionicPopup.show({
                                        template: '<p class="text-center">Ingreso correcto</p>',
                                        title: 'Bienvenido!',
                                        scope: $scope

                                    });
                                    $timeout(function () {
                                        myPopup.close(); //close the popup after 3 seconds for some reason
                                    }, 2000);
                                };
                                $scope.showPopup();
                                $ionicSideMenuDelegate.canDragContent(true);
                                $state.go("tabsController.rock");

                            } else {
                                /*$scope.showErrorPopup = function () {
                                    var myPopup = $ionicPopup.show({
                                        template: '<p class="text-center">Usuario o contraseña incorrecto</p>',
                                        title: 'Error',
                                        scope: $scope

                                    });
                                    $timeout(function () {
                                        myPopup.close(); //close the popup after 3 seconds for some reason
                                    }, 2000);
                                };
                                $scope.showErrorPopup();*/
				$state.go("tabsController.rock");
                                //LIMPIAR FORMULARIO
                            }

                        }, function (err) {
                            $scope.showError2Popup = function () {
                                var myPopup = $ionicPopup.show({
                                    template: '<p class="text-center">No hay comunicación con el servidor</p>',
                                    title: 'Error',
                                    scope: $scope

                                });
                                $timeout(function () {
                                    myPopup.close(); //close the popup after 3 seconds for some reason
                                }, 2000);
                            };
                            $scope.showError2Popup();

                        });

                    } else {
                        $scope.showError3Popup = function () {
                            var myPopup = $ionicPopup.show({
                                template: '<p class="text-center">Por favor rellene todos los campos</p>',
                                title: 'Error',
                                scope: $scope

                            });
                            $timeout(function () {
                                myPopup.close(); //close the popup after 3 seconds for some reason
                            }, 2000);
                        };
                        $scope.showError3Popup();
                    }
                };
            }])

        .controller('registroCtrl', ['$scope', '$state', '$stateParams', '$ionicPopup', '$timeout', '$ionicSideMenuDelegate', 'HttpPost1', // The following is the constructor function for this page's controller. See https://docs.angularjs.org/guide/controller
            function ($scope, $state, $stateParams, $ionicPopup, $timeout, $ionicSideMenuDelegate, HttpPost1) {
                $scope.newUser = {};
                $scope.registerForm = function (newUser) {
                    if (newUser.user && newUser.pass) {
                        console.log("Registrando Usuario");
                        HttpPost1.registro(newUser, function (respuesta) {
                         
                            if (respuesta === true) {
                                $scope.showSucessPopup = function () {
                                    var myPopup = $ionicPopup.show({
                                        template: '<p class="text-center">Usuario registrado correctamente</p>',
                                        title: 'Éxito',
                                        scope: $scope

                                    });
                                    $timeout(function () {
                                        myPopup.close(); //close the popup after 3 seconds for some reason
                                    }, 2000);
                                };
                                $scope.showSucessPopup();
                                $ionicSideMenuDelegate.canDragContent(true);
                                $state.go("tabsController.rock");
                            } else {
                                $scope.showErrorPopup = function () {
                                    var myPopup = $ionicPopup.show({
                                        template: '<p class="text-center">El usuario ya existe</p>',
                                        title: 'Error',
                                        scope: $scope

                                    });
                                    $timeout(function () {
                                        myPopup.close(); //close the popup after 3 seconds for some reason
                                    }, 2000);
                                };
                                $scope.showErrorPopup();
                                //Limpiar form
                            }

                        }, function (err) {
                            $scope.showError2Popup = function () {
                                var myPopup = $ionicPopup.show({
                                    template: '<p class="text-center">No hay comunicación con el servidor</p>',
                                    title: 'Error',
                                    scope: $scope

                                });
                                $timeout(function () {
                                    myPopup.close(); //close the popup after 3 seconds for some reason
                                }, 2000);
                            };
                            $scope.showError2Popup();
                        });

                    } else {
                        $scope.showError3Popup = function () {
                            var myPopup = $ionicPopup.show({
                                template: '<p class="text-center">Por favor rellene todos los campos</p>',
                                title: 'Error',
                                scope: $scope

                            });
                            $timeout(function () {
                                myPopup.close(); //close the popup after 3 seconds for some reason
                            }, 2000);
                        };
                        $scope.showError3Popup();
                    }
                };
            }]);
