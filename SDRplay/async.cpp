 
#include "pch.h" 
#include "async.h"
#include "libusb.h"
#include "DLL.h"
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

#include "504_s16.h"
#include "504_s8.h"
#include "384_s16.h"
#include "336_s16.h"
#include "252_s16.h"

extern Libusb libusb;

/* uložení dat */
static int mirisdr_feed_async (sdrplay_dev_t *p, uint8_t *samples, uint32_t bytes) {
    uint32_t i;

    if (!p) return -1;
    if (!p->cb) return -1;

    /* automatická velikost */
    if (!p->xfer_out_len) {
        /* přímé zaslání */
        p->cb(samples, bytes, p->cb_ctx);
    /* fixní velikost bufferu bez předchozích dat */
    } else if (p->xfer_out_pos == 0) {
        /* buffer přesně odpovídá - málo časté, přímé zaslání */
        if (bytes == p->xfer_out_len) {
            p->cb(samples, bytes, p->cb_ctx);
        /* buffer je kratší */
        } else if (bytes < p->xfer_out_len) {
            memcpy(p->xfer_out, samples, bytes);
            p->xfer_out_pos = bytes;
        /* buffer je delší */
        } else {
            /* muže být i x násobkem délky */
            for (i = 0;; i+= p->xfer_out_len) {
                if (i + p->xfer_out_len > bytes) {
                    if (bytes > i) {
                        memcpy(p->xfer_out, samples + i, bytes - i);
                        p->xfer_out_pos = bytes - i;
                    }
                    break;
                }
                p->cb(samples + i, p->xfer_out_len, p->cb_ctx);
            }
        }
    /* data jsou přesně, využije se interní buffer */
    } else if (p->xfer_out_pos + bytes == p->xfer_out_len) {
        memcpy(p->xfer_out + p->xfer_out_pos, samples, bytes);
        p->cb(p->xfer_out, p->xfer_out_len, p->cb_ctx);
        p->xfer_out_pos = 0;
    /* není dostatek dat */
    } else if (p->xfer_out_pos + bytes < p->xfer_out_len) {
        memcpy(p->xfer_out + p->xfer_out_pos, samples, bytes);
        p->xfer_out_pos+= bytes;
    /* dat je více než potřebujeme, nejsložitější případ */
    } else {
        memcpy(p->xfer_out + p->xfer_out_pos, samples, p->xfer_out_len - p->xfer_out_pos);
        p->cb(p->xfer_out, p->xfer_out_len, p->cb_ctx);
        for (i = p->xfer_out_len - p->xfer_out_pos;; i+= p->xfer_out_len) {
            if (i + p->xfer_out_len > bytes) {
                if (bytes > i) {
                    memcpy(p->xfer_out, samples + i, bytes - i);
                    p->xfer_out_pos = bytes - i;
                } else {
                    p->xfer_out_pos = 0;
                }
                break;
            }
            p->cb(samples + i, p->xfer_out_len, p->cb_ctx);
        }
    }
    return 0;

}

/* volání pro zasílání dat */
static void LIBUSB_CALL _libusb_callback(struct libusb_transfer* xfer) {
    size_t i;
    int len, bytes = 0;
    static uint8_t* iso_packet_buf;
    sdrplay_dev_t* p = (sdrplay_dev_t*)xfer->user_data;
    uint8_t* samples = NULL;
    uint32_t lost;

    if (!p) {
        Sdrplay_cancel_async(p);
    }

 


    /* zpracujeme pouze kompletní přenos */
    if (xfer->status == LIBUSB_TRANSFER_COMPLETED) {
        /*
         * Určení správné velikosti bufferu, tato část musí být provedena
         * v jednom kroku, jinak může dojít ke změně formátu uprostřed procesu,
         * druhá možnost je používat lock.
         */
        switch (xfer->type) {
        case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
            switch (p->format) {
            case format_t::MIRISDR_FORMAT_252_S16:
                //samples = (uint8_t*)malloc(504 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2);
                samples = new uint8_t[504 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2];
                for (i = 0; i < DEFAULT_ISO_PACKETS; i++) {
                    struct libusb_iso_packet_descriptor* packet = &xfer->iso_packet_desc[i];

                    /* buffer_simple je pouze pro stejně velké pakety */
                    if ((packet->actual_length > 0) &&
                        (iso_packet_buf = libusb_get_iso_packet_buffer_simple(xfer, i))) {
                        /* menší velikost než 3072 nevadí, je běžný násobek 1024, cokoliv jiného je chyba */
                        len = mirisdr_samples_convert_252_s16(p, iso_packet_buf, samples + bytes, packet->actual_length, &lost);
                        bytes += len;
                    }
                }
                break;
            case format_t::MIRISDR_FORMAT_336_S16:

               // samples = (uint8_t*)malloc(672 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2);
                samples = new uint8_t[672 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2];
                for (i = 0; i < DEFAULT_ISO_PACKETS; i++) {
                    struct libusb_iso_packet_descriptor* packet = &xfer->iso_packet_desc[i];
                    if ((packet->actual_length > 0) &&
                        (iso_packet_buf = libusb_get_iso_packet_buffer_simple(xfer, i))) {
                        len = mirisdr_samples_convert_336_s16(p, iso_packet_buf, samples + bytes, packet->actual_length, &lost);
                        bytes += len;
                    }
                }
                break;

            case format_t::MIRISDR_FORMAT_384_S16:

                //samples = (uint8_t*)malloc(768 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2);
                samples = new uint8_t[768 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2];
                for (i = 0; i < DEFAULT_ISO_PACKETS; i++) {
                    struct libusb_iso_packet_descriptor* packet = &xfer->iso_packet_desc[i];
                    if ((packet->actual_length > 0) &&
                        (iso_packet_buf = libusb_get_iso_packet_buffer_simple(xfer, i))) {
                        len = mirisdr_samples_convert_384_s16(p, iso_packet_buf, samples + bytes, packet->actual_length, &lost);
                        bytes += len;
                    }
                }

                break;
            case format_t::MIRISDR_FORMAT_504_S16:

                //samples = (uint8_t*)malloc(1008 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2);
                samples = new uint8_t[1008 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS * 2];
                for (i = 0; i < DEFAULT_ISO_PACKETS; i++) {
                    struct libusb_iso_packet_descriptor* packet = &xfer->iso_packet_desc[i];
                    if ((packet->actual_length > 0) &&
                        (iso_packet_buf = libusb_get_iso_packet_buffer_simple(xfer, i))) {
                        len = mirisdr_samples_convert_504_s16(p, iso_packet_buf, samples + bytes, packet->actual_length, &lost);
                        bytes += len;
                    }
                }
                break;
            case format_t::MIRISDR_FORMAT_504_S8:
                //samples = (uint8_t*)malloc(1008 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS);
                samples = new uint8_t[1008 * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS];
                for (i = 0; i < DEFAULT_ISO_PACKETS; i++) {
                    struct libusb_iso_packet_descriptor* packet = &xfer->iso_packet_desc[i];
                    if ((packet->actual_length > 0) &&
                        (iso_packet_buf = libusb_get_iso_packet_buffer_simple(xfer, i))) {
                        len = mirisdr_samples_convert_504_s8(p, iso_packet_buf, samples + bytes, packet->actual_length, &lost);
                        bytes += len;
                    }
                }
                break;
            }
            break;
        case LIBUSB_TRANSFER_TYPE_BULK:
            switch (p->format) {
            case format_t::MIRISDR_FORMAT_252_S16:
                samples = new uint8_t[(DEFAULT_BULK_BUFFER / 1024) * 1008];
                bytes = mirisdr_samples_convert_252_s16(p, xfer->buffer, samples, xfer->actual_length, &lost);
                break;
            case format_t::MIRISDR_FORMAT_336_S16:
                samples = new uint8_t[(DEFAULT_BULK_BUFFER / 1024) * 1344];
                bytes = mirisdr_samples_convert_336_s16(p, xfer->buffer, samples, xfer->actual_length, &lost);
                break;
            case format_t::MIRISDR_FORMAT_384_S16:
                samples = new uint8_t[(DEFAULT_BULK_BUFFER / 1024) * 1536];
                bytes = mirisdr_samples_convert_384_s16(p, xfer->buffer, samples, xfer->actual_length, &lost);
                break;
            case format_t::MIRISDR_FORMAT_504_S16:
                samples = new uint8_t[(DEFAULT_BULK_BUFFER / 1024) * 2016];
                bytes = mirisdr_samples_convert_504_s16(p, xfer->buffer, samples, xfer->actual_length, &lost);
                break;
            case format_t::MIRISDR_FORMAT_504_S8:
                samples = new uint8_t[(DEFAULT_BULK_BUFFER / 1024) * 1008];
                bytes = mirisdr_samples_convert_504_s8(p, xfer->buffer, samples, xfer->actual_length, &lost);
                break;
            }
            break;
        default:
            fprintf(stderr, "not isoc or bulk transfer type on usb device: %u\n", p->index);
            //failed:
            Sdrplay_cancel_async(p);
            /* stav failed má absolutní přednost */
            p->async_status = async_status_t::MIRISDR_ASYNC_FAILED;
        }

        if (bytes > 0) mirisdr_feed_async(p, samples, bytes);

        if (samples)
            //free(samples);
            delete(samples);

        /* pokračujeme dalším přenosem */
        if (libusb.submit_transfer(xfer) < 0) {
            fprintf(stderr, "error re-submitting URB on device %u\n", p->index);
            //failed:
            Sdrplay_cancel_async(p);
            /* stav failed má absolutní přednost */
            p->async_status = async_status_t::MIRISDR_ASYNC_FAILED;
        }
    }
    else if (xfer->status != LIBUSB_TRANSFER_CANCELLED) {
        fprintf(stderr, "error async transfer status %d on device %u\n", xfer->status, p->index);
        goto failed;
    }
 

    return;

    failed:
    Sdrplay_cancel_async(p);
    /* stav failed má absolutní přednost */
    p->async_status = async_status_t::MIRISDR_ASYNC_FAILED;
}

/* ukončení async části */
int Sdrplay_cancel_async (sdrplay_dev_t *p) {
    if (!p) goto failed;

    switch (p->async_status) {
    case async_status_t::MIRISDR_ASYNC_INACTIVE:
    case async_status_t::MIRISDR_ASYNC_CANCELING:
        goto canceled;
    case async_status_t::MIRISDR_ASYNC_RUNNING:
    case async_status_t::MIRISDR_ASYNC_PAUSED:
        p->async_status = async_status_t::MIRISDR_ASYNC_CANCELING;
        break;
    case async_status_t::MIRISDR_ASYNC_FAILED:
        goto failed;
    }

    return 0;

failed:
    return -1;

canceled:
    return -2;
}

/* ukončení async části včetně čekání */
int Sdrplay_cancel_async_now (sdrplay_dev_t *p) {
    if (!p) goto failed;

    switch (p->async_status) {
    case async_status_t::MIRISDR_ASYNC_INACTIVE:
        goto done;
    case async_status_t::MIRISDR_ASYNC_CANCELING:
        break;
    case async_status_t::MIRISDR_ASYNC_RUNNING:
    case async_status_t::MIRISDR_ASYNC_PAUSED:
        p->async_status = async_status_t::MIRISDR_ASYNC_CANCELING;
        break;
    case async_status_t::MIRISDR_ASYNC_FAILED:
        goto failed;
    }

    /* cyklujeme dokud není vše ukončeno */
    while ((p->async_status != async_status_t::MIRISDR_ASYNC_INACTIVE) &&
           (p->async_status != async_status_t::MIRISDR_ASYNC_FAILED))
#if defined (_WIN32) && !defined(__MINGW32__)
    Sleep(20);
#else
    usleep(20000);
#endif

done:
    return 0;

failed:
    return -1;
}

/* alokace asynchronních bufferů */
static int mirisdr_async_alloc (sdrplay_dev_t *p) {
    size_t i;

    if (!p->xfer) {
        //p->xfer = (libusb_transfer**)malloc(p->xfer_buf_num * sizeof(*p->xfer));
        p->xfer = new libusb_transfer * [p->xfer_buf_num];
 


        for (i = 0; i < p->xfer_buf_num; i++) {
            switch (p->transfer) {
            case transfer_t::MIRISDR_TRANSFER_BULK:
                p->xfer[i] = (libusb_transfer*)libusb.alloc_transfer(0);
                break;
            case transfer_t::MIRISDR_TRANSFER_ISOC:
                p->xfer[i] = (libusb_transfer*)libusb.alloc_transfer(DEFAULT_ISO_PACKETS);
                break;
            }
        }
    }

    if (!p->xfer_buf) {
        //p->xfer_buf = (uint8_t**)malloc(p->xfer_buf_num * sizeof(*p->xfer_buf));
        p->xfer_buf = new uint8_t* [p->xfer_buf_num];
        for (i = 0; i < p->xfer_buf_num; i++) {
            switch (p->transfer) {
            case transfer_t::MIRISDR_TRANSFER_BULK:
                p->xfer_buf[i] = new uint8_t[DEFAULT_BULK_BUFFER];
                break;
            case transfer_t::MIRISDR_TRANSFER_ISOC:
                p->xfer_buf[i] = new uint8_t[DEFAULT_ISO_BUFFER * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS];
                break;
            }
        }
    }

    if ((!p->xfer_out) &&
        (p->xfer_out_len)) {
       // p->xfer_out = (uint8_t*)malloc(p->xfer_out_len * sizeof(*p->xfer_out));
        p->xfer_out = new uint8_t[p->xfer_out_len];
    }

    return 0;
}

/* uvolnění asynchronních bufferů */
static int mirisdr_async_free (sdrplay_dev_t *p) {
    size_t i;

    if (p->xfer) {
        for (i = 0; i < p->xfer_buf_num; i++) {
            if (p->xfer[i]) libusb.free_transfer(p->xfer[i]);
        }

       // free(p->xfer);
        delete(p->xfer);
        p->xfer = NULL;
    }

    if (p->xfer_buf) {
        for (i = 0; i < p->xfer_buf_num; i++) {
            if (p->xfer_buf[i])
                delete(p->xfer_buf[i]);
        }

        delete(p->xfer_buf);
        p->xfer_buf = NULL;
    }

    if (p->xfer_out) {
       delete(p->xfer_out);
        p->xfer_out = NULL;
    }

    return 0;
}
/* spuštění async části */
int Sdrplay_read_async (sdrplay_dev_t *p, mirisdr_read_async_cb_t cb, void *ctx, uint32_t num, uint32_t len) {
    size_t i;
    int r, semafor;
    struct timeval tv = {1, 0};

    if (!p) goto failed;
    if (!p->dh) goto failed;

    /* nedovolíme spustit jiný stav než neaktivní */
    if (p->async_status != async_status_t::MIRISDR_ASYNC_INACTIVE) goto failed;

    p->cb = cb;
    p->cb_ctx = ctx;

    p->xfer_buf_num = (num == 0) ? DEFAULT_BUF_NUMBER : num;
    /* jde o fixní velikost výstupního bufferu */
    p->xfer_out_len = (len == 0) ? 0 : len;
    p->xfer_out_pos = 0;
#if MIRISDR_DEBUG >= 1
    fprintf( stderr, "async read on device %u, buffers: %lu, output size: ",
                                p->index, (long)p->xfer_buf_num);
    if (p->xfer_out_len) {
        fprintf( stderr, "%lu", (long)p->xfer_out_len);
    } else {
        fprintf( stderr, "auto");
    }
#endif
    /* použití správného rozhraní které zasílá data - není kritické */
    switch (p->transfer) {
    case transfer_t::MIRISDR_TRANSFER_BULK:
#if MIRISDR_DEBUG >= 1
        fprintf( stderr, ", transfer: bulk\n");
#endif
        if ((r = libusb.set_interface_alt_setting(p->dh, 0, 3)) < 0) {
            fprintf( stderr, "failed to use alternate setting for Bulk mode on miri usb device %u with code %d\n", p->index, r);
        }
        break;
    case transfer_t::MIRISDR_TRANSFER_ISOC:
#if MIRISDR_DEBUG >= 1
        fprintf( stderr, ", transfer: isochronous\n");
#endif
        if ((r = libusb.set_interface_alt_setting(p->dh, 0, 1)) < 0) {
            fprintf( stderr, "failed to use alternate setting for Isochronous mode on miri usb device %u with code %d\n", p->index, r);
        }
        break;
    default:
        fprintf( stderr, "\nunsupported transfer type on miri usb device %u\n", p->index);
        goto failed;
    }

    mirisdr_async_alloc(p);

    /* spustíme přenosy */
    for (i = 0; i < p->xfer_buf_num; i++) {
        switch (p->transfer) {
        case transfer_t::MIRISDR_TRANSFER_BULK:
           
            libusb.fill_bulk_transfer(p->xfer[i],
                                      p->dh,
                                      0x81,
                                      p->xfer_buf[i],
                                      DEFAULT_BULK_BUFFER,
                                      _libusb_callback,
                                      (void*) p,
                                      DEFAULT_BULK_TIMEOUT);
            break;
        case transfer_t::MIRISDR_TRANSFER_ISOC:
            libusb.fill_iso_transfer(p->xfer[i],
                                     p->dh,
                                     0x81,
                                     p->xfer_buf[i],
                                     DEFAULT_ISO_BUFFER * DEFAULT_ISO_BUFFERS * DEFAULT_ISO_PACKETS,
                                     DEFAULT_ISO_PACKETS,
                                     _libusb_callback,
                                     (void*) p,
                                     DEFAULT_ISO_TIMEOUT);
            libusb.set_iso_packet_lengths(p->xfer[i], DEFAULT_ISO_BUFFER * DEFAULT_ISO_BUFFERS);
            break;
        default:
            fprintf( stderr, "unsupported transfer type\n");
            goto failed_free;
        }

        r = libusb.submit_transfer(p->xfer[i]);

		if (r < 0) {
			fprintf(stderr, "Failed to submit transfer %lu reason: %d\n", i, r);
			goto failed_free;
		}
    }

    /* spustíme streamování dat */
    Sdrplay_streaming_start(p);

    p->async_status = async_status_t::MIRISDR_ASYNC_RUNNING;

    while (p->async_status != async_status_t::MIRISDR_ASYNC_INACTIVE) {
        /* počkáme na další událost */
        if ((r = libusb.handle_events_timeout(p->ctx, &tv)) < 0) {
            fprintf( stderr, "libusb_handle_events returned: %d\n", r);
            if (r == LIBUSB_ERROR_INTERRUPTED) continue; /* stray */
            goto failed_free;
        }

        /* dochází k ukončení */
        if (p->async_status == async_status_t::MIRISDR_ASYNC_CANCELING) {
            if (!p->xfer) {
                p->async_status = async_status_t::MIRISDR_ASYNC_INACTIVE;
                break;
            }

            /* ukončíme všechny přenosy */
            semafor = 1;
            for (i = 0; i < p->xfer_buf_num; i++) {
                if (!p->xfer[i]) continue;

                /* pro isoc režim je completed i v případě chyb */
                if (p->xfer[i]->status != LIBUSB_TRANSFER_CANCELLED) {
                    libusb.cancel_transfer(p->xfer[i]);
                    semafor = 0;
                }
            }

            /* nedošlo k žádnému vynuceném ukončení přenosu, skončíme */
            if (semafor) {
                p->async_status = async_status_t::MIRISDR_ASYNC_INACTIVE;
                /* počkáme na dokončení všech procesů */
                libusb.handle_events_timeout(p->ctx, &tv);
                break;
            }
        } else if (p->async_status == async_status_t::MIRISDR_ASYNC_FAILED) {
            goto failed_free;
        }
    }

    /* dealokujeme buffer */
    mirisdr_async_free(p);

    /* ukončíme streamování dat */
#if defined (_WIN32) && !defined(__MINGW32__)
    Sleep(20);
#else
    usleep(20000);
#endif
    Sdrplay_streaming_stop(p);
    /* je vhodné ukončit i adc, jenže pak by při dalším otevření bylo nutné provést inicializaci */

    return 0;

failed_free:
    mirisdr_async_free(p);

failed:
    return -1;
}

/* spuštění streamování */
int Sdrplay_start_async (sdrplay_dev_t *p) {
    size_t i;

    /* nedovolíme jiný stav než pozastavený */
    if (p->async_status != async_status_t::MIRISDR_ASYNC_PAUSED) 
        return -1;

    /* reset interního bufferu */
    p->xfer_out_pos = 0;

    for (i = 0; i < p->xfer_buf_num; i++) {
        if (!p->xfer[i]) continue;

        if (libusb.submit_transfer(p->xfer[i])< 0) {
            return -1;
        }
    }

    if (p->async_status != async_status_t::MIRISDR_ASYNC_PAUSED) 
        return -1;

    Sdrplay_streaming_start(p);

    p->async_status = async_status_t::MIRISDR_ASYNC_RUNNING;

    return 0;

 
}

/* zastavení streamování */
int Sdrplay_stop_async (sdrplay_dev_t *p) {
    size_t i;
    int r, semafor;
    struct timeval tv = {1, 0};

    /* nedovolíme jiný stav než spuštěný */
    if (p->async_status != async_status_t::MIRISDR_ASYNC_RUNNING) return -1;

    while (p->async_status == async_status_t::MIRISDR_ASYNC_RUNNING) {
        semafor = 1;
        for (i = 0; i < p->xfer_buf_num; i++) {
            if (!p->xfer[i]) continue;

            /* pro isoc režim je completed i v případě chyb */
            if (p->xfer[i]->status != LIBUSB_TRANSFER_CANCELLED) {
                libusb.cancel_transfer(p->xfer[i]);
                semafor = 0;
            }
        }

        if (semafor) break;

        /* počkáme na další událost */
        if ((r = libusb.handle_events_timeout(p->ctx, &tv)) < 0) {
            fprintf( stderr, "libusb_handle_events returned: %d\n", r);
            if (r == LIBUSB_ERROR_INTERRUPTED) continue; /* stray */
            return -1;
        }
    }

    if (p->async_status != async_status_t::MIRISDR_ASYNC_RUNNING) return -1;

#if defined (_WIN32) && !defined(__MINGW32__)
    Sleep(20);
#else
    usleep(20000);
#endif
    Sdrplay_streaming_stop(p);

    p->async_status = async_status_t::MIRISDR_ASYNC_PAUSED;

    return 0;

}
