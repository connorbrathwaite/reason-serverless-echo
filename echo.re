let handler: AwsLambda.APIGatewayProxy.handler =
  (event, _context, cb) => {
    open AwsLambda.APIGatewayProxy;
    let parameter =
      event
      |. Event.queryStringParametersGet
      /* andThen : ('a -> 'b option [@bs]) -> 'a option -> 'b option */
      /* get dict key returns None if the key is not found in the dictionary, Some value otherwise */
      |> Js.Option.andThen((. params) => Js.Dict.get(params, "userid"));
    switch (parameter) {
    | Some(userid) => Js.log2("[Debug] Executing lambda for", userid)
    | None => Js.log("[Debug] Executing lambda for anonymous user")
    };
    let result =
      switch (event |. Event.bodyGet) {
      | None =>
        Js.log("[Error] Empty event payload");
        result(
          ~body=`Plain({|{"status": "Empty event payload"}|}),
          ~statusCode=400,
          (),
        );
      | Some(body) =>
        Result.make(
          ~body,
          /* TOFIX */
          /* ~headers=Js_dict.t(Js.Json.t), */
          ~isBase64Encoded=event |. Event.isBase64EncodedGet,
          ~statusCode=200,
        )
      };
    cb(Js.null, result);
    Js.Promise.resolve();
  };