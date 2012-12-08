void PADpoll_multitap( int value )
{
	if( pad_count == 0 ) {
		pad_cmd = value;
	}


	switch( pad_cmd ) {
		// $42 = poll data
		case 0x42:
			// already done
			if( pad_count > 0 ) {
				// poll pad1
				if( pad_count == 2 ) {
					pad_out[2+0] = 0x41;
					pad_out[2+1] = 0x5a;
					pad_out[2+2] = 0xff;
					pad_out[2+3] = 0xff;

					pad_out[2+4] = 0xff;
					pad_out[2+5] = 0xff;
					pad_out[2+6] = 0xff;
					pad_out[2+7] = 0xff;


					{
						static int test = 0;
						static int test2 = 0;

						test++;
						if( test < 20 ) {
							if(test2 == 1 )
							// RIGHT - flip 3/4
							pad_out[2+2] = ~0x20;

							else

							// LEFT - flip
							pad_out[2+2] = ~0x80;
						}
						else if( test < 40 ) {
						}
						else if( test < 60 && test2 == 0 ) {
							// X (triggers DOWN?)
							pad_out[2+3] = ~0x40;

							// TRIANGLE
							//pad_out[2+3] = ~0x10;

							// START
							pad_out[2+2] = ~0x08;
						}
						else if( test < 80 ) {
							//test2 = 1;
						}
						else if( test >= 100 ) {
							test = 0;
						}
					}


					// cmd - illegal
					if( value != 0x42 ) {
						// Frogger 2 - invalid (only $ff works!)
						pad_out[2+1] = 0xff;

						// dummy data
						pad_out[2+2] = 0xff;
						pad_out[2+3] = 0xff;
					}
				}


				// poll pad2
				if( pad_count == 10 ) {
					pad_out[10+0] = 0x41;
					pad_out[10+1] = 0x5a;
					pad_out[10+2] = 0xff;
					pad_out[10+3] = 0xff;

					pad_out[10+4] = 0xff;
					pad_out[10+5] = 0xff;
					pad_out[10+6] = 0xff;
					pad_out[10+7] = 0xff;


					{
						static int test = 0;

						test++;
						if( test < 20 ) {
							// RIGHT - flip 3/4
							//pad_out[10+2] = ~0x20;

							// LEFT - flip
							pad_out[10+2] = ~0x80;
						}
						else if( test < 40 ) {
						}
						else if( test < 60 ) {
							// X (triggers DOWN?)
							pad_out[10+3] = ~0x40;

							// TRIANGLE
							//pad_out[10+3] = ~0x10;

							// START
							pad_out[10+2] = ~0x08;
						}
						else if( test < 80 ) {
						}
						else if( test >= 100 ) {
							test = 0;
						}
					}



					// cmd - illegal
					if( value != 0x42 ) {
						// Frogger 2 - invalid (only $ff works!)
						pad_out[10+1] = 0xff;

						// dummy data
						pad_out[10+2] = 0xff;
						pad_out[10+3] = 0xff;
					}
				}


				// poll pad3
				if( pad_count == 18 ) {
					pad_out[18+0] = 0x41;
					pad_out[18+1] = 0x5a;
					pad_out[18+2] = 0xff;
					pad_out[18+3] = 0xff;

					pad_out[18+4] = 0xff;
					pad_out[18+5] = 0xff;
					pad_out[18+6] = 0xff;
					pad_out[18+7] = 0xff;



					{
						static int test = 0;

						test++;
						if( test < 20 ) {
							// RIGHT - flip 3/4
							//pad_out[18+2] = ~0x20;

							// LEFT - flip
							pad_out[18+2] = ~0x80;
						}
						else if( test < 40 ) {
						}
						else if( test < 60 ) {
							// X (triggers DOWN?)
							pad_out[18+3] = ~0x40;

							// TRIANGLE
							//pad_out[18+3] = ~0x10;

							// START
							pad_out[18+2] = ~0x08;
						}
						else if( test < 80 ) {
						}
						else if( test >= 100 ) {
							test = 0;
						}
					}



					// cmd - illegal
					if( value != 0x42 ) {
						// Frogger 2 - invalid (only $ff works!)
						pad_out[18+1] = 0xff;

						// dummy data
						pad_out[18+2] = 0xff;
						pad_out[18+3] = 0xff;
					}
				}


				// poll pad4
				if( pad_count == 26 ) {
					pad_out[26+0] = 0x41;
					pad_out[26+1] = 0x5a;
					pad_out[26+2] = 0xff;
					pad_out[26+3] = 0xff;

					pad_out[26+4] = 0xff;
					pad_out[26+5] = 0xff;
					pad_out[26+6] = 0xff;
					pad_out[26+7] = 0xff;


					{
						static int test = 0;

						test++;
						if( test < 20 ) {
							// RIGHT - flip 3/4
							//pad_out[26+2] = ~0x20;

							// LEFT - flip
							pad_out[26+2] = ~0x80;
						}
						else if( test < 40 ) {
						}
						else if( test < 60 ) {
							// X (triggers DOWN?)
							pad_out[26+3] = ~0x40;

							// TRIANGLE
							//pad_out[26+3] = ~0x10;

							// START
							pad_out[26+2] = ~0x08;
						}
						else if( test < 80 ) {
						}
						else if( test >= 100 ) {
							test = 0;
						}
					}



					// cmd - illegal
					if( value != 0x42 ) {
						// Frogger 2 - invalid (only $ff works!)
						pad_out[26+1] = 0xff;

						// dummy data
						pad_out[26+2] = 0xff;
						pad_out[26+3] = 0xff;
					}
				}
				break;
			}

			// ==================================
			// ==================================

			// reset config mode
			if( pad_cmd == 0x42 ) {
				pad_config_mode[ THIS_PAD ] = 0;
			}


			// multitap ID
			pad_out[0] = 0x80;
			pad_out[1] = 0x5a;

			// ==================================
			// ==================================

			// retrieve 8 bytes of pad1
			// retrieve 8 bytes of pad2
			// retrieve 8 bytes of pad3
			// retrieve 8 bytes of pad4
			break;


		// ENTER_CONFIG_MODE
		// - Rampage 2 - Universal Tour
		case 0x43:
			// already done
			if( pad_count > 0 ) {
				// set config mode
				if( pad_count == 2 )
				{
					pad_config_mode[ THIS_PAD ] = value;
				}
				break;
			}

			// ==================================
			// ==================================

			// multitap ID
			pad_out[0] = 0x80;
			pad_out[1] = 0x5a;
			break;


		// QUERY_MODEL
		// - Rampage 2 - Universal Tour
		case 0x45:
			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x80;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				// - Rampage 2 (must be >= ~$3e each - some type of math)
				pad_out[2] = 0xff;	// model
				pad_out[3] = 0xff;	// # of modes

				pad_out[4] = 0x00;	// mode cur offset
				pad_out[5] = 0x00;	// # of act
				pad_out[6] = 0x00;	// # of comb
				pad_out[7] = 0x00;
			}
			break;


		// QUERY_COMB
		// - Rampage 2 - Universal Tour
		case 0x47:
			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x80;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;
				pad_out[3] = 0x00;

				pad_out[4] = 0x01;
				pad_out[5] = 0x00;
				pad_out[6] = 0x00;
				pad_out[7] = 0x00;
			}
			break;


		// QUERY_MODE
		// - Rampage 2 - Universal Tour
		case 0x4c:
			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x80;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;
				pad_out[3] = 0x00;

				pad_out[4] = 0x00;	// hi-mode
				pad_out[5] = 0x80;	// lo-mode

				pad_out[6] = 0x00;
				pad_out[7] = 0x00;
			}
			break;


		default:
			// already done
			if( pad_count > 0 ) break;


			// invalid command
			pad_out[0] = 0x80;
			pad_out[1] = 0xff;
			break;
	}
}
