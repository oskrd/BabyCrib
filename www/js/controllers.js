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

	    $scope.request1 = function () {
                HttpGet.getSonido().then(function (response) {
                    $scope.crying = response;
                    console.log('Sonido ->', $scope.crying);
                });
            };
            $scope.request1();

	    $scope.request2 = function () {
                HttpGet.getProximidad().then(function (response) {
                    $scope.proximidad = response;
                    console.log('proximidad ->', $scope.proximidad);
                });
            };
            $scope.request2();

            if ($scope.proximidad === 0 && $scope.crying === "1"){
                $scope.movList=  [{text: "Estado Mecedor", checked: true}];
            } else{
                $scope.movList=  [{text: "Estado Mecedor", checked: false}];
            }
            $ionicLoading.hide();

            $scope.request = function () {
                HttpPost2.mov({"valor": $scope.cambiar($scope.movList[0].checked)}, function (data) {
                    console.log(data);
                }, function (err){console.log(err)});
            }
                      
	$ionicLoading.hide();
})

        .controller('sonidoCtrl', function ($scope, $ionicLoading, HttpGet) {
	    $scope.soundList = [{text: "Sonido en la cuna", icon: "icon ion-android-volume-off"}];
	    $ionicLoading.show({
                template: 'Cargando...'
            });
            $scope.request = function () {
                HttpGet.getSonido().then(function (response) {
                    $scope.crying = response;
                    console.log('Sonido ->', $scope.crying);
                    if ($scope.crying === "0") {
                        $scope.soundList[0].icon = "icon ion-android-volume-off";
                    } else {
                        $scope.soundList[0].icon = "icon ion-android-volume-up";
                    }
                    $ionicLoading.hide();
                });
            };
            $scope.request();
//$scope.doorList = [{text: "Puerta principal", icon: "icon ion-android-volume-off"},
  //                      {text: "Terraza", icon: "icon ion-android-volume-up"},{text: "Puerta principal", icon: "icon ion-android-checkmark-circle"},
 //                       {text: "Terraza", icon: "icon ion-close-circled"}];
        })

	.controller('proxCtrl', function ($scope, $ionicLoading, HttpGet) {
	    $scope.proximityList = [{text: "Bebé en la cuna", icon: "icon ion-close-circled"}];
	    $ionicLoading.show({
                template: 'Cargando...'
            });
            $scope.request = function () {
                HttpGet.getProximidad().then(function (response) {
                    $scope.proximidad = response;
                    //console.log('proximidad ->', $scope.proximidad);
                    if ($scope.proximidad === 0) {
                        $scope.proximityList[0].icon = "icon ion-close-circled";
                    } else {
                        $scope.proximityList[0].icon = "icon ion-android-checkmark-circle";
                    }
                    $ionicLoading.hide();
                });
            };
            $scope.request();
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
