<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Charger_model extends CI_Model {

	public $ID;
	public $UID;
	public $CustomerID;
	public $State;
	public $Created = '0000-00-00 00:00:00';
	
    function __construct()
    {
        // Call the Model constructor
        parent::__construct();
    }
    
    function getcustomer($params = array()){
        $q = $this->db->select('CustomerID, State, Created')->from('Charger')->where('Charger.UID', $params['UID'])->order_by('Created','desc')->limit(1);
        
        $filters = array();
		$filters = $params;
		
        
        $response = $this->api_model->load($q, $filters, 'Charger');
    
        return $response;
    }

	function create(){
		
        $insert = array(
                'UID'      			=> $this->UID,
				'CustomerID'		=> $this->CustomerID,
				'State'				=> $this->State,
				'Created'			=> $this->Created
        );

        if($this->db->insert('Charger',$insert)){
            $response = array('ID' => $this->db->insert_id());
        }else{
            $response = array('error' => 'Could not create customer');
        }
        return $response;
        
    }

}

// end of model file
