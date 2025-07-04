#include "klevebrand-skywire-http-stepper-client.h"

bool SkywireHttpStepperClient::start()
{
    skywire.start();
}

SkywireResponseResult_t SkywireHttpStepperClient::get(String path)
{
    if (!httpCfg())
    {
        SkywireResponseResult_t result(false, "");

        return result;
    }

    if (!httpQry(path))
    {
        SkywireResponseResult_t result(false, "");

        return result;
    }

    if (!httpRing())
    {
        SkywireResponseResult_t result(false, "");

        return result;
    }

    if (!httpRcv())
    {
        SkywireResponseResult_t result(false, "");

        return result;
    }

    if (skywire.available())
    {
        String response_content = skywire.readString();

        SkywireResponseResult_t result(true, response_content);

        http_cfg_ok_recieved = false;
        http_cfg_sent = false;
        http_qry_ok_recieved = false;
        http_qry_sent = false;
        http_ring_recieved = false;
        http_rcv_ok_recieved = false;
        http_rcv_sent = false;

        return result;
    }

    SkywireResponseResult_t result(false, "");

    return result;
}

bool SkywireHttpStepperClient::httpCfg()
{
    if (!http_cfg_sent)
    {
        skywire.print("AT#HTTPCFG=0,\"" + base_url + "\"\r");

        http_cfg_sent = true;

        return false;
    }

    if (http_cfg_sent && !http_cfg_ok_recieved)
    {
        if (Serial3.available())
        {
            String response = Serial3.readString();

            Serial.println("STEPPER CLIENT RECIEVED HTTPCFG OK");

            http_cfg_ok_recieved = true;

            return true;
        }

        return false;
    }

    if (http_cfg_sent && http_cfg_ok_recieved)
    {
        return true;
    }

    return false;
}

bool SkywireHttpStepperClient::httpQry(String path)
{
    if (!http_qry_sent)
    {
        skywire.print("AT#HTTPQRY=0,0,\"/" + path + "\"\r");

        http_qry_sent = true;

        return false;
    }

    if (http_qry_sent && !http_qry_ok_recieved)
    {
        if (Serial3.available())
        {
            String response = Serial3.readString();

            Serial.println("STEPPER CLIENT RECIEVED HTTPQRY OK");

            http_qry_ok_recieved = true;

            return true;
        }

        return false;
    }

    if (http_qry_sent && http_qry_ok_recieved)
    {
        return true;
    }

    return false;
}

bool SkywireHttpStepperClient::httpRing()
{
    if (!http_ring_recieved)
    {
        if (Serial3.available())
        {
            String response = Serial3.readString();

            http_ring_recieved = response.indexOf("HTTPRING") > 0;

            if (http_ring_recieved)
            {
                Serial.println("STEPPER CLIENT RECIEVED HTTPRING OK");

                return true;
            }
        }
    }

    return false;
}

bool SkywireHttpStepperClient::httpQry(String path)
{
    if (!http_rcv_sent)
    {
        skywire.print("AT#HTTPRCV=0");

        http_qry_sent = true;

        return false;
    }

    if (http_qry_sent && !http_qry_ok_recieved)
    {
        if (Serial3.available())
        {
            String response = Serial3.readString();

            Serial.println("STEPPER CLIENT RECIEVED HTTPQRY OK");

            http_qry_ok_recieved = true;

            return true;
        }

        return false;
    }

    if (http_qry_sent && http_qry_ok_recieved)
    {
        return true;
    }

    return false;
}