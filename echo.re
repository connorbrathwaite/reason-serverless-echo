let handler: AwsLambda.APIGatewayProxy.handler =
  (event, _context, cb) => {
    open AwsLambda.APIGatewayProxy;
    let parameter =
      event
      |. Event.queryStringParametersGet
      |> Js.Option.andThen((. params) => Js.Dict.get(params, "userid"));
    switch (parameter) {
    | Some(userid) => Js.log2("[DEBUG]: Executing lambda for", userid)
    | None => Js.log("[DEBUG]: Executing lambda for anonymous user")
    };
    let result =
      switch (event |. Event.bodyGet) {
      | None =>
        Js.log("[ERROR]: No body available in the request");
        result(
          ~body=`Plain({|{"status": "No body available in the request"}|}),
          ~statusCode=400,
          (),
        );
      | Some(body) =>
        Js.log("[DEBUG]: Body " ++ body);
        Js.log(event |. Event.isBase64EncodedGet);
        Result.make(
          ~statusCode=200,
          ~body,
          ~isBase64Encoded=event |. Event.isBase64EncodedGet,
          (),
        );
      };
    cb(Js.null, result);
    Js.Promise.resolve();
  };